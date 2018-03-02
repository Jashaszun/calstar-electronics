// Access control: CalSTAR MR

#include <avr/io.h>
#include <util/delay.h>
#include <Arduino.h>
#include <Wire.h>
#include <SparkFunMPL3115A2.h>

#define F_CPU 16000000
#define RECEIVER_PIN 3
#define TRANSMITTER_PIN 2
#define BLACK_POWDER_PIN_ARDUINO A1
#define LED_PIN_RED 5
#define LED_PIN_GREEN 7
#define LED_PIN_BLUE 6
#define BUZZER_PIN 9
#define CONTINUITY_PIN 4
#define ACCEL_TOLERANCE 1
#define ALT_LAND 200
#define ALT_LAUNCHED 200
#define ACCEL_ADDR 0x00
#define ALT_ADDR 0x60
#define ALT_WRITE_ADDR 0xC0
#define ALT_READ_ADDR 0xC1
#define VERIF_SAMPLES 20

// Altimeter
MPL3115A2 altimeter;

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
	DEPLOYED
};

char wait_for_signal();

void process_beep(); // runs each iteration of the while loop and flips voltage if necessary
void beep_if_continuity(); // every 2 seconds, check for continuity - if there's continuity, beep for half a second
void start_beep(long duration_us); // runs a beep that lasts for `duration` ms,
								   // if another call is made that will override any existing beep
void stop_beep();

short black_powder_present();
short deployment_signal();
void setLEDs(uint8_t red, uint8_t green, uint8_t blue);
short launched();
short landed();

short state = INIT;
short launch_state = PAD;

float base_alt = altimeter.readAltitudeFt(); // used for zeroing altitude reading

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

	// Configure accelerometer
	Wire.begin();
	altimeter.begin();
	altimeter.setModeAltimeter();
	altimeter.setOversampleRate(7);
	altimeter.enableEventFlags();

	Serial.println("Deployment board started.");

	bool printAltimeterReading = false;

	while (1) {
		switch (state) {
			case INIT:
				base_alt = 0;
				for (int i = 0; i < 100; i++) {
					base_alt += altimeter.readAltitudeFt();
				}
				base_alt /= 100;
				state = TEST_MODE;
				break;
			case TEST_MODE:
				if (printAltimeterReading) {
					Serial.print("Altimeter reading: ");
					Serial.println(altimeter.readAltitudeFt());
				}
				if (command_available) {
					if (command == "EXIT\n") {
						Serial.println("Deployment exiting test mode.");
						state = INIT;
					} else if (command.length() == 8 && command.substring(0, 4) == "led ") {
						setLEDs(command[4] == '1', command[5] == '1', command[6] == '1');
					} else if (command == "LVDS_RECEIVE\n") {
						Serial.print("LVDS received: ");
						Serial.println(digitalRead(RECEIVER_PIN));
					} else if (command.length() == 4 && command[0] == 'T') {
						// should be a space after the T
						Serial.println("Toggling transmitter pin");
						digitalWrite(TRANSMITTER_PIN, command[2] == '1');
					} else if (command == "BP_ON\n") {
						Serial.println("BP = ON");
						digitalWrite(BLACK_POWDER_PIN_ARDUINO, 1);
					} else if (command == "BP_OFF\n") {
						Serial.println("BP = OFF");
						digitalWrite(BLACK_POWDER_PIN_ARDUINO, 0);
					} else if (command == "CONTINUITY\n") {
						Serial.println(black_powder_present() ? "Continuity - black powder present" : "Disconnected - black powder absent");
					} else if (command == "BEEP\n") {
						Serial.print("Starting beep for 0.5s");
						start_beep(500000);
						Serial.println("done.");
					} else if (command == "ALTIMETER") {
						printAltimeterReading = !printAltimeterReading;
						Serial.print(printAltimeterReading ? "P" : "Not p");
						Serial.println("rinting altimeter reading");
					}
					resetCommand();
				}
				break;
			case LAUNCH:
				beep_if_continuity();
				switch(launch_state) {
					case PAD:
						if (launched()) launch_state = FLIGHT;
						break;
					case FLIGHT:
						setLEDs(1, 0, 0); // indicates launched condition met
						if (landed()) launch_state = LANDED;
						break;
					case LANDED:
						setLEDs(0, 1, 0); // indicates landing condition met
						// TODO add wait_for_signal process
						if (deployment_signal()) launch_state = SIGNAL_RECEIVED;
						break;
					case SIGNAL_RECEIVED:
						setLEDs(0, 0, 1); // indicates receipt of signal
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
		  Serial.println("Received command: " + command);
		  if (state == LAUNCH)
		  	Serial.println("Launch mode.");
		  else Serial.println("Test mode.");
		}

		// Switch into test mode with serial input
		if (command_available && command == "test\n" && state != TEST_MODE) {
		  state = TEST_MODE;
		  setLEDs(0, 0, 0);
		  Serial.println("Deployment entering test mode.");
		  resetCommand();
		}
	}
}

char wait_for_signal() {
	short signal_received = 0;
	int count = 0;
	while (!signal_received) { // wait for signal from ejection
		if (deployment_signal()) { // low voltage means signal!
			count++;
			if (count >= VERIF_SAMPLES) { // need five signals to verify
				signal_received = 1; // break loop and return
			}
		} else {
			count = 0;
		}
		beep_if_continuity();
	}
	if (landed()) { // landed!
		return 1;
	}
	return 0; // not on ground
}

// ** PROCESSING BEEPING **
bool beeping = false; // whether or not process_beep should attempt to emit noise
unsigned long last_buzzer_flip = 0; // the time of the last flip of the buzzer voltage
unsigned long last_beep_start = 0; // the most recent time where start_beep was called
unsigned long curr_beep_duration = 0; // the duration in us of the current beep
bool buzzer_state = 0; // the next voltage to be written to the buzzer pin

/*
This function should be called on every iteration of the while loop.
If a beep does not need to be done, then it just returns.
Otherwise, if enough time has passed since the last flip of the buzzer voltage, it will flip the
voltage on the buzzer. 
*/
void process_beep() {
	if (beeping && micros() - last_buzzer_flip >= 500) { // beeps at 500 Hz
		buzzer_state = !buzzer_state;
		last_buzzer_flip = micros();
		digitalWrite(BUZZER_PIN, buzzer_state);
	}
	if (micros() - last_beep_start > curr_beep_duration) {
		stop_beep();
	}
}

void start_beep(long duration_us) {
	beeping = true;
	last_beep_start = micros();
	curr_beep_duration = duration_us;
}

void stop_beep() {
	beeping = false;
}

unsigned long last_continuity_check = 0; // last time continuity check was performed
void beep_if_continuity() {
	if (micros() - last_continuity_check >= 2000000 // check every two seconds
			&& black_powder_present()) { // continuity detected
		start_beep(500000); // beep for 0.5 seconds
	}
}

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
		counter ++;
		return (counter >= VERIF_SAMPLES);
	}
}

short landed() {
	static unsigned short counter = 0;
	if (altimeter.readAltitudeFt() - base_alt > ALT_LAND) {
		counter = 0;
		return 0;
	} else {
		counter ++;
		return (counter >= VERIF_SAMPLES);
	}
}
