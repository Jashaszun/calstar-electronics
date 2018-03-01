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
#define VERIF_SAMPLES 5
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

enum State {
  INIT, // Blue only
  LAUNCH_PAD, // Green only
  LAUNCHED, // Blue Green
  RADIO_WAIT, // Red only
  DEPLOY, // Red Blue
  LVDS_WAIT, // Red Green
  SCISSOR_LIFT_ACTIVATE, // Red Green Blue
  TEST_MODE,
  DISABLED
};

short launched(MPL3115A2 &altimeter, float altZero);
short landed(MPL3115A2 &altimeter, float altZero);
short deploymentDisconnect();

// Sets RGB LED pins to these respective states (HIGH or LOW)
void setLEDs(uint8_t red, uint8_t green, uint8_t blue);

bool command_available = false;
String command = "";
void resetCommand();

bool enableTelemetry = true;

int main() {
  init(); // always call this first if using Arduino.h!
  Wire.begin(); // for I2C
  // Create peripherals
  MPL3115A2 altimeter;
#ifdef ENABLE_ATC
  RFM69_ATC radio;
#else
  RFM69 radio;
#endif
  Servo servo;
  LIS331HH accelerometer(0);
  unsigned long lastTransmitTime = 0;
  float altMovingAvg = 0;
  float currentAltZero = 0;

  pinMode(RECEIVER_PIN, INPUT_PULLUP);
  pinMode(TRANSMITTER_PIN, OUTPUT);
  pinMode(LED_PIN_BLUE, OUTPUT);
  pinMode(LED_PIN_RED, OUTPUT);
  pinMode(LED_PIN_GREEN, OUTPUT);

  digitalWrite(TRANSMITTER_PIN, LOW);
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
        setLEDs(HIGH, LOW, HIGH);
        // TRANSMITTER_PORT &= ~(1 << TRANSMITTER_PIN); // send deployment signal
        digitalWrite(TRANSMITTER_PIN, HIGH);
        state = LVDS_WAIT;
        break;
      case LVDS_WAIT:
        setLEDs(HIGH, HIGH, LOW);
        if (deploymentDisconnect()) {
          state = SCISSOR_LIFT_ACTIVATE;
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
            Serial.println("Ejection exiting test mode.");
          }
          else if (command == "z") {
            currentAltZero = altMovingAvg;
            Serial.println("Zeroing altitude");
          }
          else if (command == "lvds r") {
            Serial.println("LVDS Receive: " + digitalRead(RECEIVER_PIN));
          }
          else if (command == "lvds on") {
            digitalWrite(TRANSMITTER_PIN, HIGH);
            Serial.println("LVDS Transmit set to on.");
          }
          else if (command == "lvds off") {
            digitalWrite(TRANSMITTER_PIN, LOW);
            Serial.println("LVDS Transmit set to off.");
          }
          else if (command == "t") {
            enableTelemetry = !enableTelemetry;
            if (enableTelemetry) {
              Serial.println("Enabling telemetry");
            } else {
              Serial.println("Disabling telemetry");
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
          else {
            Serial.print("Invalid command received: ");
            Serial.println(command);
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
			sprintf(outbuf, "Ac: x: %+07i y: %+07i z: %+07i  Al: %+07i\n",
				(int)(1000*accelX), (int)(1000*accelY), (int)(1000*accelZ), (int)(alt-currentAltZero));

			radio.send(TRANSMIT_TO, outbuf, strlen(outbuf));

      #define ALPHA (.75f)
			altMovingAvg = ALPHA*altMovingAvg + (1-ALPHA)*alt;

			lastTransmitTime = millis();
    }


    // Read serial and radio, check to enter test mode
    if (Serial.available()) {
      command_available = true;
      command = Serial.readString();
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
      Serial.println("Ejection entering test mode.");
      resetCommand();
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
  for (int i = 0; i < VERIF_SAMPLES; i++) {
    if (digitalRead(RECEIVER_PIN) == LOW) { // RECIEVER_PIN is pulled up, so when it is LOW that means deployment is transmitting that
      return 0;
    }
    // if (!(RECEIVER_PORT & (1 << RECEIVER_PIN))) {
    //   return 0;
    // }
    delay(10);
  }
  return 1;
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


// // Turn on red LED
// LED_PORT |= 1 << LED_PIN_RED;
//
// // Turn off red LED
// LED_PORT &= ~(1 << LED_PIN_RED);
