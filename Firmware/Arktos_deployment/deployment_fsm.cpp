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
	LAUNCH = 0,
	TEST_MODE = 1,
};

enum LaunchState {
	PAD = 0,
	FLIGHT = 1,
	SIGNAL_RECEIVED = 2,
	DEPLOYED = 3
};

char wait_for_signal();

void beep();

short black_powder_present();
short deployment_signal();
void setLEDs(uint8_t red, uint8_t green, uint8_t blue);
short launched();
short landed();

short state = LAUNCH;
short launch_state = PAD;

float base_alt = altimeter.readAltitudeFt(); // used for zeroing altitude reading
float* base_alt_ptr = &base_alt; // so all functions can manipulate this

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
			case TEST_MODE:
				if (printAltimeterReading) {
					Serial.print("Altimeter reading: ");
					Serial.println(altimeter.readAltitudeFt());
				}
				if (command_available) {
					if (command == "EXIT\n") {
						Serial.println("Deployment exiting test mode.");
						state = LAUNCH;
						*base_alt_ptr = altimeter.readAltitudeFt(); // make sure we re-establish base altitude when we switch back into LAUNCH mode
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
						Serial.print("Beeping... ");
						Serial.flush();

						for (int i = 0; i < 5; i++) beep();

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
				switch(launch_state) {
					case PAD:
						*base_alt_ptr = (*base_alt_ptr + altimeter.readAltitudeFt()) / 2; // moving exponential average filter
						setLEDs(1, 1, 0);
						for (int i = 0; i < 3; i++) {
							setLEDs(1, 1, 1);
							beep();
							setLEDs(1, 1, 0);
							delay(1000);
						}
						setLEDs(0, 1, 1);
						for (int i = 0; i < 3; i++) {
							if (black_powder_present()) beep();
							else delay(1000);
						}
						launch_state = FLIGHT;
						break;
					case FLIGHT:
						setLEDs(1, 0, 0); // red LEDs indicate signal never received
						if (deployment_signal()) launch_state = SIGNAL_RECEIVED;
						break;
					case SIGNAL_RECEIVED:
						setLEDs(0, 1, 0); // green LEDs indicate receipt of signal
						// trigger black powder
						digitalWrite(BLACK_POWDER_PIN_ARDUINO, 1);
						delay(1000);
						digitalWrite(BLACK_POWDER_PIN_ARDUINO, 0);
						launch_state = DEPLOYED;
						break;
					case DEPLOYED:
						setLEDs(0, 0, 1); // program complete
						if (black_powder_present()) {
							beep();
						}
						delay(1000);
						break;
				}
				break;
		}
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
		if (black_powder_present()) {
			beep();
		}
	}
	if (landed()) { // landed!
		return 1;
	}
	return 0; // not on ground
}

void beep() {
	uint8_t buzzer = 0;
	unsigned long start_time_us = micros();
	unsigned long last_flip = start_time_us;
	//digitalWrite(BUZZER_PIN, buzzer);
	while (micros() - start_time_us < 1000000) {
		if (micros() - last_flip >= 500) {
			buzzer = !buzzer;
			last_flip = micros();
			digitalWrite(BUZZER_PIN, buzzer);
			//BUZZER_PORT = (buzzer << BUZZER_PIN);
		}
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
	// return !(RECEIVER_PORT & (1 << RECEIVER_PIN));
	return !digitalRead(RECEIVER_PIN);
}

short launched() {
	return altimeter.readAltitudeFt() - *base_alt_ptr > ALT_LAUNCHED;
}

short landed() {
	return altimeter.readAltitudeFt() - *base_alt_ptr < ALT_LAND;
}
