// Access control: CalSTAR MR

#include <avr/io.h>
#include <util/delay.h>
#include <Arduino.h>
// #include <Wire.h>
// #include <SparkFunMPL3115A2.h>

#define F_CPU 16000000
#define RECEIVER_IO DDRD
#define RECEIVER_PORT PIND
#define RECEIVER_PIN 3
#define TRANSMITTER_IO DDRD
#define TRANSMITTER_PORT PORTD
#define TRANSMITTER_PIN 2
#define CHARGE_IO DDRC
#define CHARGE_PORT PORTC
#define CHARGE_PIN 1
#define BLACK_POWDER_PIN_ARDUINO A1
#define LED_IO DDRD
#define LED_PORT PORTD
#define LED_PIN_RED 5
#define LED_PIN_GREEN 7
#define LED_PIN_BLUE 6
#define BUZZER_IO DDRB
#define BUZZER_PORT PORTB
#define BUZZER_PIN 1
#define CONTINUITY_IO DDRD
#define CONTINUITY_PORT PIND
#define CONTINUITY_PIN 4
#define ACCEL_TOLERANCE 1
#define ALT_LAND 200
#define ALT_LAUNCHED 200
#define ACCEL_ADDR 0x00
#define ALT_ADDR 0x60
#define ALT_WRITE_ADDR 0xC0
#define ALT_READ_ADDR 0xC1
#define BASE_ALT 0
#define VERIF_SAMPLES 20

// Altimeter
// MPL3115A2 altimeter;

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
	// LVDS receiver and continuity default to input
	// LED_IO = (1 << LED_PIN_RED) | (1 << LED_PIN_GREEN) | (1 << LED_PIN_BLUE); // configure LEDs as output
	// TRANSMITTER_IO |= (1 << TRANSMITTER_PIN); // configure LVDS transmitter as output
	//CHARGE_IO = 0xFF; // configure black powder as output
	BUZZER_IO = 0xFF; // configure buzzer as output
	// LED_PORT = 0x00; // set LEDs and transmitter output to low
	//CHARGE_PORT = 0x00; // set charge to low
	BUZZER_PORT = 0x00; // set buzzer to low

	pinMode(RECEIVER_PIN, INPUT_PULLUP);
	pinMode(TRANSMITTER_PIN, OUTPUT);
	pinMode(LED_PIN_BLUE, OUTPUT);
	pinMode(LED_PIN_GREEN, OUTPUT);
	pinMode(LED_PIN_RED, OUTPUT);
	digitalWrite(TRANSMITTER_PIN, LOW);
	pinMode(BLACK_POWDER_PIN_ARDUINO, OUTPUT);
	digitalWrite(BLACK_POWDER_PIN_ARDUINO, LOW);

	// Configure accelerometer
	// Wire.begin();
	// altimeter.begin();
	// altimeter.setModeAltimeter();
	// altimeter.setOversampleRate(7);
	// altimeter.enableEventFlags();

	Serial.println("Deployment board started.");

	while (1) {
		switch (state) {
			case TEST_MODE:
				if (command_available) {
					if (command == "EXIT") {
						Serial.println("Deployment exiting test mode.");
						state = LAUNCH;
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
						digitalWrite(BLACK_POWDER_PIN_ARDUINO, HIGH);
					} else if (command == "BP_OFF") {
						Serial.println("BP = OFF");
						digitalWrite(BLACK_POWDER_PIN_ARDUINO, LOW);
					} else if (command == "CONTINUITY") {
						Serial.println(black_powder_present() ? "Continuity - black powder present" : "Disconnected - black powder absent");
					} else if (command == "BEEP") {
						Serial.print("Beeping... ");
						Serial.flush();

						for (int i = 0; i < 5; i++) beep();

						Serial.println("done.");
					}
					resetCommand();
				}
				break;
			case LAUNCH:
				switch(launch_state) {
					case PAD:
						setLEDs(LOW, LOW, LOW);
						for (int i = 0; i < 3; i++) {
							beep();
							delay(1000);
						}
						state = FLIGHT;
						break;
					case FLIGHT:
						setLEDs(HIGH, LOW, LOW); // red LEDs indicate signal never received
						if (deployment_signal()) launch_state = SIGNAL_RECEIVED;
						break;
					case SIGNAL_RECEIVED:
						setLEDs(LOW, HIGH, LOW); // green LEDs indicate receipt of signal
						// trigger black powder
						digitalWrite(BLACK_POWDER_PIN_ARDUINO, HIGH);
						delay(1000);
						digitalWrite(BLACK_POWDER_PIN_ARDUINO, LOW);
						launch_state = DEPLOYED;
						break;
					case DEPLOYED:
						setLEDs(LOW, LOW, HIGH); // program complete
						if (black_powder_present()) {
							beep();
						}
						delay(1000);
						break;
				}
		}
		// Read serial and radio, check to enter test mode
		if (Serial.available()) {
		  command_available = true;
		  command = Serial.readString();
		}

		if (command_available && command == "test" && state != TEST_MODE) {
		  state = TEST_MODE;
		  setLEDs(LOW, LOW, LOW);
		  Serial.println("Ejection entering test mode.");
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
	short buzzer = 0;
	unsigned long start_time_us = micros();
	while (micros() - start_time_us < 1000000) {
		buzzer = (micros() % 1000000 < 500000) ? 0 : 1;
		digitalWrite(BUZZER_PIN, buzzer);
	}
}

short black_powder_present() {
	return !(CONTINUITY_PORT & (1 << CONTINUITY_PIN));
}

// Turn on/off 3 LEDs
void setLEDs(uint8_t red, uint8_t green, uint8_t blue) {
    digitalWrite(LED_PIN_RED, red);
    digitalWrite(LED_PIN_GREEN, green);
    digitalWrite(LED_PIN_BLUE, blue);
}

short deployment_signal() {
	// return !(RECEIVER_PORT & (1 << RECEIVER_PIN));
	return digitalRead(RECEIVER_PIN) == LOW;
}

short launched() {
	return 1;
}

short landed() {
	return 1;
}
