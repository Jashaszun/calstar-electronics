// Access control: CalSTAR MR

#include <avr/io.h>
#include <util/delay.h>
#include <Arduino.h>
#include <Wire.h>
#include <SparkFunMPL3115A2.h>
#include <LIS331HH.h>

#define F_CPU 16000000
#define RECEIVER_PIN 3
#define TRANSMITTER_PIN 2
#define BLACK_POWDER_PIN_ARDUINO A1
#define LED_PIN_RED 5
#define LED_PIN_GREEN 7
#define LED_PIN_BLUE 6
#define BUZZER_PIN 9
#define CONTINUITY_PIN 4
//#define ACCEL_TOLERANCE 1
#define ALT_LAND 200
#define ALT_LAUNCHED 200
#define ACCEL_ADDR 0x00
#define ALT_ADDR 0x60
#define ALT_WRITE_ADDR 0xC0
#define ALT_READ_ADDR 0xC1
#define VERIF_SAMPLES 20 // number of samples needed to verify landing
#define ACCEL_TOLERANCE .3 // maximum acceptable acceleration magnitude margin (in gs)

#define BITLEN 100
#include <JankConnection.h> // Could it get jankier?

// Altimeter
MPL3115A2 altimeter;
// accelerometer
LIS331HH accelerometer(0);

enum State {
  INIT,
  LAUNCH,
  TEST_MODE
};

enum LaunchState {
  PAD,
  FLIGHT,
  LANDED,
  SIGNAL_RECEIVED,
  TRIGGER,
  DEPLOYED
};

char wait_for_signal();

// seconds to microseconds
//#define S_TO_US(N) ((unsigned long)(N)*1000000)
// milliseconds to microseconds
#define MS_TO_US(N) ((unsigned long)(N)*1000)
/* --- Nonblocking beep functions --- */
bool beeping = false; // whether or not process_beep should attempt to emit noise
void process_beep(); // runs each iteration of the while loop and flips voltage if necessary
//void beep_if_continuity(); // every 2 seconds, check for continuity - if there's continuity, beep for half a second
void start_beep(unsigned long beep_ms, unsigned long pause_ms); // runs a beep that lasts for beep_ms, and pauses for pause_ms
                                   // if another call is made that will override any existing beep
void stop_beep(); // stops the currently running beep, if there is one.

/* --- Nonblocking delay functions --- */
/*bool delaying = false;
void process_delay(); // runs each iteration of the while loop and stops delay if finished
void start_delay(unsigned long ms); // delays for that many milliseconds. additional calls override existing delays.
void stop_delay(); // stops the currently running delay, if there is one.*/

short black_powder_present();
short deployment_signal();
void setLEDs(uint8_t red, uint8_t green, uint8_t blue);
short launched();
short landed();

short state = INIT;
short launch_state = PAD;

float base_alt = 0; // used for zeroing altitude reading

float accel_stable = 10; // Stable when near 0

bool command_available = false;
String command = "";
void resetCommand() {
  command_available = false;
  command = "";
}

int main() {
  /* Pseudocode for deployment sequence:
  1. Verify vehicle has launched with altimeter
  2. Wait until ADC input exceeds ADC threshold
  3. Verify vehicle is at rest on the ground with altimeter
  4. Actuate solenoid. */

  // Setup
  init(); // always do this when using Arduino.h!
  Serial.begin(19200);

  pinMode(RECEIVER_PIN, INPUT_PULLUP);
  pinMode(TRANSMITTER_PIN, OUTPUT);
  pinMode(LED_PIN_BLUE, OUTPUT);
  pinMode(LED_PIN_GREEN, OUTPUT);
  pinMode(LED_PIN_RED, OUTPUT);
  digitalWrite(TRANSMITTER_PIN, 0);
  pinMode(BLACK_POWDER_PIN_ARDUINO, OUTPUT);
  digitalWrite(BLACK_POWDER_PIN_ARDUINO, 0);

  pinMode(BUZZER_PIN, OUTPUT);

  // Configure altimeter
  Wire.begin();
  altimeter.begin();
  altimeter.setModeAltimeter();
  altimeter.setOversampleRate(7);
  altimeter.enableEventFlags();

  // Configure accelerometer
  accelerometer.write_reg(LIS331HH_CTRL_REG1, 0x3F); // Normal power mode, 1000 Hz data rate, x y and z axes enabled
  accelerometer.write_reg(LIS331HH_CTRL_REG4, 0xB0); // Block data update, +- 12g scale


  Serial.println("Deployment board started.");

  bool printAltimeterReading = false;
  bool printAccelerometerReading = false;

  int num_init_beeps = 0;
  int current_altimeter_zeroing_reading = 0;
  int num_zero_done_beeps = 0;
  while (1) {
    switch (state) {
      case INIT:
      	// run 3 of [1 second beep, 1 second delay]
      	if (num_init_beeps < 3) {
          if (!beeping) {
      	  	start_beep(1000, 1000); // 1s beep, 1s pause
      	  	num_init_beeps++;
            Serial.print("Beeping ");
            Serial.println(num_init_beeps);
          }
      	} else if (current_altimeter_zeroing_reading < 100) {
          // collect 100 measurements to zero but delay in between each one
          if (!beeping) {
            start_beep(0, 50); // 50 ms between readings // Side note this is like the jankiest timing I've ever seen in my life
            setLEDs(current_altimeter_zeroing_reading < 50, current_altimeter_zeroing_reading % 50 < 25, current_altimeter_zeroing_reading % 25 < 12);
            base_alt += altimeter.readAltitudeFt();
            current_altimeter_zeroing_reading++;
            if (current_altimeter_zeroing_reading % 10 == 0) {
              Serial.print("Altimeter reading ");
              Serial.println(current_altimeter_zeroing_reading);
            }
          }
        } else if (num_zero_done_beeps < 3) {
          if (!beeping) {
            start_beep(250, 250); // 250 ms beep, 250 ms pause
            num_zero_done_beeps++;
          }
        } else {
	        base_alt /= 100;
          Serial.print("Base Altitude: ");
          Serial.println(base_alt);
          state = LAUNCH;
          launch_state = PAD;
        }
        break;
      case TEST_MODE:
        if (printAltimeterReading) {
          Serial.print("Altimeter reading: ");
          Serial.println(altimeter.readAltitudeFt());
        }
        if (printAccelerometerReading) {
          float ax = accelerometer.get_x_g();
          float ay = accelerometer.get_y_g();
          float az = accelerometer.get_z_g();
          Serial.print("Accelerometer reading: (x: ");
          Serial.print(ax, 2);
          Serial.print(", y: ");
          Serial.print(ay, 2);
          Serial.print(", z: ");
          Serial.print(az, 2);
          Serial.print("), Mag=");
          Serial.println(ax*ax + ay*ay + az*az);
        }
        if (command_available) {
          if (command == "EXIT") {
            Serial.println("Deployment exiting test mode.");
            state = INIT;
          } else if (command == "SIGNAL_RECEIVED") {
            Serial.println("Moving to SIGNAL_RECEIVED.");
            state = LAUNCH;
            launch_state = SIGNAL_RECEIVED;
          } else if (command.length() == 7 && command.substring(0, 4) == "led ") {
            setLEDs(command[4] == '1', command[5] == '1', command[6] == '1');
          } else if (command == "LVDS_RECEIVE") {
            Serial.print("LVDS received: ");
            Serial.println(digitalRead(RECEIVER_PIN));
          } else if (command.length() == 3 && command[0] == 'T') {
            // should be a space after the T
            Serial.println("Toggling transmitter pin");
            digitalWrite(TRANSMITTER_PIN, command[2] == '1');
          } else if (command == "BP_ON") {
            Serial.println("BP = ON");
            digitalWrite(BLACK_POWDER_PIN_ARDUINO, 1);
          } else if (command == "BP_OFF") {
            Serial.println("BP = OFF");
            digitalWrite(BLACK_POWDER_PIN_ARDUINO, 0);
          } else if (command == "CONTINUITY") {
            Serial.println(black_powder_present() ? "Continuity - black powder present" : "Disconnected - black powder absent");
          } else if (command == "BEEP") {
            Serial.println("Starting beep for 0.5s");
            start_beep(500, 0);
          } else if (command == "ALTIMETER") {
            printAltimeterReading = !printAltimeterReading;
            Serial.print(printAltimeterReading ? "P" : "Not p");
            Serial.println("rinting altimeter reading");
          } else if (command == "ACCEL") {
            printAccelerometerReading = !printAccelerometerReading;
            Serial.print(printAccelerometerReading ? "P" : "Not p");
            Serial.println("rinting accelerometer reading");
            // Serial.print("Printing accelerometer (x y z): (");
            // Serial.print(accelerometer.get_x_g());
            // Serial.print(accelerometer.get_y_g());
            // Serial.print(accelerometer.get_z_g());
            // Serial.println(")");
          } else if (command == "LANDED") {
            for (int landed_try = 0; landed_try < 2*VERIF_SAMPLES; landed_try++) {
              if (landed()) Serial.println("Landed.");
              else Serial.println("Not landed.");
            }
          } else if (command == "MCUCR") {
            Serial.println(MCUCR);
          } else if (command.length() > 5 && command.substring(0, 5) == "jank ") {
            uint8_t val = atoi(command.c_str() + 5);
            Serial.println("Sending over jank: " + String(command.c_str() + 5));
            Serial.println(startMessageSend);
            sendMessage(val);
          } else {
            Serial.println("Invalid command.");
          }
          resetCommand();
        }
        break;
      case LAUNCH:
        // beep for .5s every 2s if continuity at the start of the period
        if (!beeping) {
          if (black_powder_present()) {
            start_beep(500, 1500);
          }
          else {
            start_beep(0, 2000);
          }
        }
        switch(launch_state) {
          case PAD:
          	setLEDs(deployment_signal(), 0, 1);
            if (launched()) launch_state = FLIGHT;
            break;
          case FLIGHT:
            setLEDs(1, 0, 0); // indicates launched condition met
            if (landed()) launch_state = LANDED;
            break;
          case LANDED:
            setLEDs(0, 1, 0); // indicates landing condition met
            if (wait_for_signal()) {
              launch_state = SIGNAL_RECEIVED;
            }
            break;
          case SIGNAL_RECEIVED:
            setLEDs(0, 1, 1); // indicates receipt of signal
            // Wait for stable accelerometer values
            {
              static unsigned long lastmillis = 0;
              if (millis() > lastmillis + 100) {
                lastmillis = millis();
                float xval = accelerometer.get_x_g();
                float yval = accelerometer.get_y_g();
                float zval = accelerometer.get_z_g();

                float norm_squared = xval * xval + yval * yval + zval * zval;
                float stability = abs(norm_squared - 1);

                // Serial.print(stability);
                // Serial.print("   ");
                // Serial.println(accel_stable);

                accel_stable = accel_stable * 0.5 + stability * 0.5; // Running average

                if (accel_stable < 0.2) {
                  launch_state = TRIGGER;
                }
              }
            }

            break;
          case TRIGGER:
            setLEDs(1, 0, 1); // Triggering
            // trigger black powder
            digitalWrite(BLACK_POWDER_PIN_ARDUINO, 1);
            delay(1000);
            digitalWrite(BLACK_POWDER_PIN_ARDUINO, 0);
            launch_state = DEPLOYED;
            break;
          case DEPLOYED:
            setLEDs(1, 1, 1); // program complete
            break;
        }
        break;
    }
    // check if a beep should be made
    process_beep();

    // Read serial and radio, check to enter test mode
    if (Serial.available()) {
      command_available = true;
      command = Serial.readString();
      if (command.endsWith("\n")) {
        command = command.substring(0, command.length() - 1);
      }
    }

    // Switch into test mode with serial input
    if (command_available && state != TEST_MODE) {
      if (command == "test") {
        state = TEST_MODE;
        setLEDs(0, 0, 0);
        Serial.println("Deployment entering test mode.");
        resetCommand();
      } else {
        Serial.println("Not in test mode.");
        resetCommand();
      }
    }

    processConnection();
    if (receivedMessage) {
      switch (messageReceiving) {
        default:
        {
          char to_print[10];
          itoa(messageReceiving, to_print, 10);
          Serial.println("Received code: " + String(to_print));
        }
          break;
      }
      receivedMessage = false;
    }
  }
}

char wait_for_signal() {
  static unsigned short count = 0;
  if (deployment_signal() && landed()) { // low voltage means signal!
    count++;
    return (count >= VERIF_SAMPLES);
  } else {
    count = 0;
    return 0;
  }
  // because beep_if_continuity is run on each iteration of the main loop anyway,
  // beep_if_continuity() does not need to be called here
}

// ** PROCESSING BEEPING **
unsigned long last_buzzer_flip = 0; // the time of the last flip of the buzzer voltage
unsigned long last_beep_start = 0; // the most recent time, in ms, where start_beep was called
unsigned long curr_beep_duration = 0; // the duration in ms of the current beep
unsigned long curr_pause_duration = 0; // the duration in ms of the current pause (post-beep)
bool buzzer_state = 0; // the next voltage to be written to the buzzer pin

/*
This function should be called on every iteration of the while loop.
If a beep does not need to be done, then it just returns.
Otherwise, if enough time has passed since the last flip of the buzzer voltage, it will flip the
voltage on the buzzer.
*/
void process_beep() {
  unsigned long current_time_ms = millis();
  if (millis() - last_beep_start > curr_beep_duration) {
    digitalWrite(BUZZER_PIN, LOW);
  }
  if (millis() - last_beep_start > curr_beep_duration + curr_pause_duration) {
    beeping = false;
  }
}

void start_beep(unsigned long beep_ms, unsigned long pause_ms) {
  beeping = true;
  last_beep_start = millis();
  curr_beep_duration = beep_ms;
  curr_pause_duration = pause_ms;
  if (beep_ms != 0) {
    analogWrite(BUZZER_PIN, 127);
  }
}

/*// *** Processing delaying ***
unsigned long last_delay_start; // most recent time start_delay was called
unsigned long curr_delay_duration; // duration in ms of current delay
void start_delay(unsigned long ms) {
	delaying = true;
	last_delay_start = millis();
	curr_delay_duration = ms;
}
void process_delay() {
	if (millis() - last_delay_start >= curr_delay_duration)
		stop_delay();
}
void stop_delay() {
	delaying = false;
}


unsigned long last_continuity_check = 0; // last time continuity check was performed
void beep_if_continuity() {
  if (micros() - last_continuity_check >= 2000000 // check every two seconds
      && black_powder_present()) { // continuity detected
    start_beep(500000); // beep for 0.5 seconds
  }
}*/

short black_powder_present() {
  return !digitalRead(CONTINUITY_PIN);
}

// Turn on/off 3 LEDs
void setLEDs(uint8_t red, uint8_t green, uint8_t blue) {
  digitalWrite(LED_PIN_RED, red);
  digitalWrite(LED_PIN_GREEN, green);
  digitalWrite(LED_PIN_BLUE, blue);
}

short deployment_signal() {
  return !digitalRead(RECEIVER_PIN);
}

short launched() {
  static unsigned short counter = 0;
  if (altimeter.readAltitudeFt() - base_alt < ALT_LAUNCHED) {
    counter = 0;
    return 0;
  } else {
    counter++;
    return (counter >= VERIF_SAMPLES);
  }
}

short landed() {
  static unsigned short counter = 0;
  // "Landed" means altimeter says we've landed AND accelerometer says we're not moving
  float ax = accelerometer.get_x_g();
  float ay = accelerometer.get_y_g();
  float az = accelerometer.get_z_g();

  float mag = ax*ax + ay*ay + az*az;
  float margin = mag - 1;
  if (margin < 0) margin = -margin;

  if (altimeter.readAltitudeFt() - base_alt > ALT_LAND
    || margin > ACCEL_TOLERANCE) {
      //|| accelerometer.get_x_g() > ACCEL_TOLERANCE
      //|| accelerometer.get_y_g() > ACCEL_TOLERANCE
      //|| accelerometer.get_z_g() > ACCEL_TOLERANCE) {

    counter = 0;
    return 0;
  } else {
    counter++;
    return (counter >= VERIF_SAMPLES);
  }
}
