// Access control: CalSTAR MR

#include <avr/io.h>
#include <util/delay.h>
#include <Arduino.h>
#include <Wire.h>
#include <SparkFunMPL3115A2.h>
#include <LIS331HH.h>
#include <RFM69.h>
#include <Servo.h>

#define F_CPU 16000000
#define RECEIVER_IO DDRD
#define RECEIVER_PORT PIND
#define RECEIVER_PIN 7
#define TRANSMITTER_IO DDRB
#define TRANSMITTER_PORT PORTB
#define TRANSMITTER_PIN 8 // 0 PB0
#define LED_IO DDRD
#define LED_PORT PORTD
#define LED_PIN_RED 17 // 3
#define LED_PIN_GREEN 15 // 1
#define LED_PIN_BLUE 16 // 2
#define ALT_LAND 200
#define ALT_LAUNCHED 200
#define VERIF_SAMPLES 20
#define SERVO_PIN 6
#define RADIO_RESET_PIN A0
#define NODEID (4)
#define NETWORKID (100)
#define TRANSMIT_TO (255) // broadcast
#define FREQUENCY RF69_433MHZ
#define ENCRYPTKEY "sampleEncryptKey"
#define DEPLOY_SIGNAL "DEPLOY"
#define RADIO_SIGNAL_LEN strlen(DEPLOY_SIGNAL)
#define TRANSMIT_INTERVAL 200 // ms
#define EJECT_WAIT_TIME 2000 // ms
#define BITLEN 100 // ms

#include <JankConnection.h> // Could it get jankier?

enum State {
  INIT = 0, // Blue only
  LAUNCH_PAD = 1, // Green only
  LAUNCHED = 2, // Blue Green
  RADIO_WAIT = 3, // Red only
  DEPLOY = 4, // Red Blue
  LVDS_WAIT = 5, // Red Green
  SCISSOR_LIFT_ACTIVATE = 6, // Red Green Blue
  TEST_MODE = 7,
  DISABLED = 8
};

const char* state_names[] = {"IN", "PD", "LD", "RW", "DP", "LW", "SL", "TM", "DS"};

// Create peripherals
MPL3115A2 altimeter;
Servo servo;
LIS331HH accelerometer(0);
#ifdef ENABLE_ATC
  RFM69_ATC radio;
#else
  RFM69 radio;
#endif

short launched(MPL3115A2 &altimeter, float altZero);
short landed(MPL3115A2 &altimeter, float altZero);
short deploymentDisconnect();

// Sets RGB LED pins to these respective states (HIGH or LOW)
void setLEDs(uint8_t red, uint8_t green, uint8_t blue);

bool command_available = false;
String command = "";
void resetCommand();

void log(String message);

bool enableTelemetry = true;


int main() {
  init(); // always call this first if using Arduino.h!
  Wire.begin(); // for I2C
  unsigned long lastTransmitTime = 0;
  float altMovingAvg = 0;
  float currentAltZero = 0;

  pinMode(RECEIVER_PIN, INPUT_PULLUP);
  pinMode(TRANSMITTER_PIN, OUTPUT);
  pinMode(LED_PIN_BLUE, OUTPUT);
  pinMode(LED_PIN_RED, OUTPUT);
  pinMode(LED_PIN_GREEN, OUTPUT);

  digitalWrite(TRANSMITTER_PIN, HIGH);
  // Set outputs and inputs; receiver defaults to input
  // LED_IO = (1 << LED_PIN_RED) | (1 << LED_PIN_GREEN) | (1 << LED_PIN_BLUE); // set LEDs as outputs
  // TRANSMITTER_IO = 0xFF; // set LVDS transmitter as output
  // TRANSMITTER_PORT |= (1 << TRANSMITTER_PIN);

  pinMode(RADIO_RESET_PIN, OUTPUT);

  Serial.begin(19200);
  Serial.println("Ejection board started");

  State state = INIT;
  State prevState;

  while (true) {
    switch (state) {
      case INIT:
        // Initialize
        setLEDs(LOW, LOW, HIGH);

        altimeter.begin();
        altimeter.setModeAltimeter();
        altimeter.setOversampleRate(7);
        altimeter.enableEventFlags();

        currentAltZero = 0;
        for (int i = 0; i < 100; i++) {
          currentAltZero += altimeter.readAltitudeFt() / 100;
          delay(50);
        }

        digitalWrite(RADIO_RESET_PIN, HIGH);
        delay(500);
        digitalWrite(RADIO_RESET_PIN, LOW);
        delay(500);

        radio.initialize(FREQUENCY, NODEID, NETWORKID);
        radio.setHighPower();
        radio.encrypt(ENCRYPTKEY);
        #ifdef ENABLE_ATC
        radio.enableAutoPower(ATC_RSSI)
        #endif

        servo.attach(SERVO_PIN);

        // CHANGE
        //  |
        //  |
        //  |
        //  |
        // \/
        servo.write(180); // NEEDS TO BE CHANGED <--------- CHANGE
        // ^
        // |
        // |
        // |
        // |
        // CHANGE


        accelerometer.write_reg(LIS331HH_CTRL_REG1, 0x3F); // Normal power mode, 1000 Hz data rate, x y and z axes enabled
        accelerometer.write_reg(LIS331HH_CTRL_REG4, 0xB0); // Block data update, +- 12g scale

        state = LAUNCH_PAD;
        break;
      case LAUNCH_PAD:
        setLEDs(LOW, HIGH, LOW);

        if (launched(altimeter, currentAltZero)) {
          state = LAUNCHED;
        }
        if (command_available && command == "z") {
          currentAltZero = altMovingAvg;
          resetCommand();
        }
        break;
      case LAUNCHED:
        setLEDs(LOW, HIGH, HIGH);
        if (landed(altimeter, currentAltZero)) {
          state = RADIO_WAIT;
        }
        break;
      case RADIO_WAIT:
        setLEDs(HIGH, LOW, LOW);

        if (command_available && command == DEPLOY_SIGNAL) {
          state = DEPLOY;
          resetCommand();
        }
        break;
      case DEPLOY:
      {
        static bool deployed = false;
        setLEDs(HIGH, LOW, HIGH);
        // TRANSMITTER_PORT &= ~(1 << TRANSMITTER_PIN); // send deployment signal
        // digitalWrite(TRANSMITTER_PIN, LOW);
        if (!deployed) {
          sendMessage(0xF0);
          deployed = true;
        }

        if (!sendingMessage) {
          state = LVDS_WAIT;
        }
      }
        break;
      case LVDS_WAIT:
        setLEDs(HIGH, HIGH, LOW);
        {
          static bool timerstarted = false;
          static unsigned int starttime;
          if (deploymentDisconnect()) {
            if (!timerstarted) {
              starttime = millis();
              timerstarted = true;
            }
            if (starttime + EJECT_WAIT_TIME < millis()) {
              state = SCISSOR_LIFT_ACTIVATE;
            }
          } else {
            timerstarted = false;
          }
        }
        break;
      case SCISSOR_LIFT_ACTIVATE:
        setLEDs(HIGH, HIGH, HIGH);
        // NEED TO WRITE
        // NEED TO WRITE
        // NEED TO WRITE
        // NEED TO WRITE
        servo.write(0);
        // NEED TO WRITE
        // NEED TO WRITE
        // NEED TO WRITE
        state = DISABLED;
        break;
      case TEST_MODE:
        if (command_available) {
          if (command == "INIT") {
            state = INIT;
          }
          else if (command == "LAUNCH_PAD") {
            state = LAUNCH_PAD;
          }
          else if (command == "LAUNCHED") {
            state = LAUNCHED;
          }
          else if (command == "RADIO_WAIT") {
            state = RADIO_WAIT;
          }
          else if (command == "DEPLOY") {
            state = DEPLOY;
          }
          else if (command == "LVDS_WAIT") {
            state = LVDS_WAIT;
          }
          else if (command == "SCISSOR_LIFT_ACTIVATE") {
            state = SCISSOR_LIFT_ACTIVATE;
          }
          else if (command == "DISABLED") {
            state = DISABLED;
          }
          else if(command == "EXIT") {
            state = prevState;
            log("Ejection exiting test mode.");
          }
          else if (command == "z") {
            currentAltZero = altMovingAvg;
            log("Zeroing altitude");
          }
          else if (command == "lvds r") {
            log("LVDS Receive: " + digitalRead(RECEIVER_PIN));
          }
          else if (command == "lvds on") {
            digitalWrite(TRANSMITTER_PIN, HIGH);
            log("LVDS Transmit set to on.");
          }
          else if (command == "lvds off") {
            digitalWrite(TRANSMITTER_PIN, LOW);
            log("LVDS Transmit set to off.");
          }
          else if (command == "telemetry") {
            enableTelemetry = !enableTelemetry;
            if (enableTelemetry) {
              log("Enabling telemetry");
            } else {
              log("Disabling telemetry");
            }
          }
          else if (command.length() > 6 && command.substring(0, 6) == "radio ") {
            const char* tempstr = command.c_str() + 6;
            Serial.println("Sending over radio: " + String(tempstr));
            radio.send(TRANSMIT_TO, tempstr, strlen(tempstr));
          }
          else if (command.length() > 7 && command.substring(0, 7) == "serial ") {
            Serial.println(command.substring(7));
          }
          else if (command.length() == 7 && command.substring(0, 4) == "led ") {
            setLEDs(command[4] == '1', command[5] == '1', command[6] == '1');
          }
          else if (command.length() > 6 && command.substring(0, 6) == "servo ") {
            servo.write(atoi(command.substring(6).c_str()));
          }
          else if (command.length() > 5 && command.substring(0, 5) == "jank ") {
            uint8_t val = atoi(command.c_str() + 5);
            log("Sending over jank: " + String(command.c_str() + 5));
            sendMessage(val);
          }
          else {
            log("Invalid command received: " + command);
          }
          resetCommand();
        }


      case DISABLED:
        break;
    }

    // Send Telemetry
    if (millis() - lastTransmitTime >= TRANSMIT_INTERVAL && enableTelemetry) {
            float accelX = accelerometer.get_x_g();
			float accelY = accelerometer.get_y_g();
			float accelZ = accelerometer.get_z_g();
			float alt = altimeter.readAltitudeFt();

			char outbuf[100];
			sprintf(outbuf, "St: %s   Ac: x: %+07i y: %+07i z: %+07i   Al: %+07i\n", state_names[state],
				(int)(1000*accelX), (int)(1000*accelY), (int)(1000*accelZ), (int)(alt-currentAltZero));

			radio.send(TRANSMIT_TO, outbuf, strlen(outbuf));
      Serial.println(outbuf);

      #define ALPHA (.75f)
			altMovingAvg = ALPHA*altMovingAvg + (1-ALPHA)*alt;

			lastTransmitTime = millis();
    }


    // Read serial and radio, check to enter test mode
    if (Serial.available()) {
      command_available = true;
      command = Serial.readString();
      if (command.endsWith("\n")) {
        command = command.substring(0, command.length() - 1);
      }
    }
    if (radio.receiveDone()) {
      if (radio.ACKRequested()) {
        radio.sendACK();
      }
      command_available = true;
      command = "";
      for (int i = 0; i < radio.DATALEN - 1; i++) {
        command += (char)radio.DATA[i];
      }
      Serial.print("Received command: ");
      Serial.println(command);
    }

    if (command_available && command == "test" && state != TEST_MODE) {
      prevState = state;
      state = TEST_MODE;
      setLEDs(LOW, LOW, LOW);
      log("Ejection entering test mode.");
      resetCommand();
    }

    processConnection();

    if (receivedMessage) {
      switch (messageReceiving) {
        case 0x0F:
          log("Acked BP activate");
          break;
        case 0x17:
          log("Acked BP activate 1s");
          break;
        case 0x2B:
          log("Acked BP deactivate");
          break;
        case 0x33:
          log("Received discontinuous");
          break;
        case 0x4d:
          log("Received continuous");
          break;
        case 0x55:
          log("Acked beep");
          break;
        case 0x69:
          log("Deployment state: INIT");
          break;
        case 0x71:
          log("Deployment state: PAD");
          break;
        case 0x8E:
          log("Deployment state: FLIGHT");
          break;
        case 0x96:
          log("Deployment state: LANDED");
          break;
        case 0xAA:
          log("Deployment state: SIGNAL_RECEIVED");
          break;
        case 0xB2:
          log("Deployment state: TRIGGER");
          break;
        case 0xCC:
          log("Deployment state: DEPLOYED");
          break;
        case 0xD4:
          log("Acked altitude checks off");
          break;
        case 0xE8:
          log("Acked altitude checks on");
          break;
        case 0xF0:
          log("Acked deployment triggered");
          break;
        default:
        {
          char to_print[10];
          itoa(messageReceiving, to_print, 10);
          log("Received code: " + String(to_print));
        }
          break;
      }
      receivedMessage = false;
    }

  }
}

short launched(MPL3115A2 &altimeter, float altZero) {
	static int count = 0;
	if (altimeter.readAltitudeFt() - altZero > ALT_LAUNCHED) {
		count++;
		if (count >= VERIF_SAMPLES) { // need five signals to verify
			return 1; // yay we launched
		}
	} else {
		count = 0;
	}
	return 0;
}

short landed(MPL3115A2 &altimeter, float altZero) {
	for (int i = 0; i < VERIF_SAMPLES; i++) {
		if (altimeter.readAltitudeFt() - altZero > ALT_LAND) {
			return 0; // if any of the first five signals disagree, return - resend radio signal to retrigger
		}
	}
	return 1;
}

short deploymentDisconnect() {
  static unsigned short counter = 0;
  if (digitalRead(RECEIVER_PIN) == HIGH) {
    counter += 1;
    if (counter >= VERIF_SAMPLES) return 1;
    else return 0;
  } else {
    counter = 0;
    return 0;
  }
}

// Turn on/off 3 LEDs
void setLEDs(uint8_t red, uint8_t green, uint8_t blue) {
    digitalWrite(LED_PIN_RED, red);
    digitalWrite(LED_PIN_GREEN, green);
    digitalWrite(LED_PIN_BLUE, blue);
}

void resetCommand() {
  command_available = false;
  command = "";
}

void log(String message) {
  Serial.println(message);
  const char* tempstr = message.c_str();
  radio.send(TRANSMIT_TO, tempstr, message.length());
}
