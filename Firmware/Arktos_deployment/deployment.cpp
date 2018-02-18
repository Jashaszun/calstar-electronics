// Access control: CalSTAR MR

#include <avr/io.h>
#include <util/delay.h>
#include <Arduino.h>
//#include <Wire.h>
//#include <SparkFunMPL3115A2.h>

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

char waitForSignal();

void start_beep(unsigned short);
void beep();
void stop_beep();
bool beep_for_ms(unsigned long, unsigned short);
void start_beep_then_pause(unsigned long, unsigned long, unsigned short);
bool beep_then_pause();

short detectContinuity();
short launched(int alt);
short landed();
short deploymentSignal();
void setLEDs(uint8_t red, uint8_t green, uint8_t blue);

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

	// Initialize timer vars
	//buzzer_time = micros();

	// Main program thread
	// while (!launched(altimeter.readAltitudeFt())) { // wait for vehicle to launch
	// 	beep();
	// }
	// set LED to red to indicate launch
	setLEDs(HIGH, HIGH, HIGH);

	while (beep_for_ms(500, 880)) { }
	setLEDs(HIGH, LOW, LOW);

	// Wait 5 seconds, check for test mode in that time
	unsigned long start_time = millis();
	while (millis() - start_time < 5000) {
		if (Serial.available() && (Serial.readString() == "test")) {
			Serial.println("Deployment entering test mode.");
			// Test mode loop
			while (true) {
				if (!Serial.available()) {
					continue;
				}
				String command = Serial.readString();
				if (command == "EXIT") {
					Serial.println("Deployment exiting test mode.");
					break;
				}
				else if (command.length() == 7 && command.substring(0, 4) == "led ") {
					setLEDs(command[4] == '1', command[5] == '1', command[6] == '1');
				}
				else if (command == "LVDS_RECEIVE") {
					Serial.print("LVDS received: ");
					Serial.println(digitalRead(RECEIVER_PIN));
				}
				else if (command.length() == 3 && command[0] == 'T') {
					// should be a space after the T
					digitalWrite(TRANSMITTER_PIN, command[2] == '1');
				}
				else if (command == "BP_ON") {
					Serial.println("BP = ON");
					digitalWrite(BLACK_POWDER_PIN_ARDUINO, HIGH);
				}
				else if (command == "BP_OFF") {
					Serial.println("BP = OFF");
					digitalWrite(BLACK_POWDER_PIN_ARDUINO, LOW);
				} else if (command == "CONTINUITY") {
					Serial.println(detectContinuity() ? "Continuity" : "Disconnected");
				} else if (command == "BEEP") {
					Serial.print("Beeping... ");
					Serial.flush();

					while (beep_for_ms(1000, 800)) { }

					Serial.println("done.");
				}
			}
			break;
		}
	}

	setLEDs(LOW, HIGH, HIGH);
	while (waitForSignal() == 0) { // wait for ejection signal and cross-check with sensor
		//if (!beep_then_pause()) { // if done beeping then pausing, then ...
		//	if (detectContinuity()) { // check for continuity...
		//		start_beep_then_pause(500, 1500, 900); // and if so, then start a beep then pause
		//	}
		//}
	}
	// turn red LED off
	setLEDs(LOW, LOW, LOW);
	// set LED to green to indicate receipt of signal
	setLEDs(LOW, HIGH, LOW);

	// trigger black powder
	digitalWrite(BLACK_POWDER_PIN_ARDUINO, HIGH);
	delay(1000);
	digitalWrite(BLACK_POWDER_PIN_ARDUINO, LOW);

	// turn green LED off
	setLEDs(LOW, LOW, LOW);
	// set LED to blue to indicate completion of program
	setLEDs(LOW, LOW, HIGH);
	while (true) {
		//if (!beep_then_pause()) { // if done beeping then pausing, then ...
		//	if (detectContinuity()) { // check for continuity...
		//		start_beep_then_pause(500, 1500, 900); // and if so, then start a beep then pause
		//		Serial.println("Beeping then pausing.");
		//	}
		//}
	}
	return 0;
}

char waitForSignal() {
	short signal_received = 0;
	int count = 0;
	while (!signal_received) { // wait for signal from ejection
		if (deploymentSignal()) { // low voltage means signal!
			count++;
			if (count >= VERIF_SAMPLES) { // need five signals to verify
				signal_received = 1; // break loop and return
			}
		} else {
			count = 0;
		}
		if (detectContinuity()) {
			beep();
		}
	}
	if (landed()) { // landed!
		return 1;
	}
	return 0; // not on ground
}

//unsigned long beep_start_time_ms;
unsigned long buzzer_on_start_time_us;
unsigned long buzzer_period_us;
bool beeping = false;
void start_beep(unsigned short frequency) {
	buzzer_on_start_time_us = micros();
	buzzer_period_us = 500000 / frequency; // frequency = 1/(2*period_us)
	beeping = true;
}
void beep() {
	static short buzzer = 0;
	//unsigned long current_time_ms = millis();
	unsigned long current_time_us = micros();
	//if (current_time_ms - start_time >= 1000) { // Counts up to one second then resets
	//	start_time = current_time_ms;
	//}
	if (current_time_us - buzzer_on_start_time_us >= buzzer_period_us) { // Alternate every period_us us.
		buzzer_on_start_time_us = current_time_us;
		// buzzer frequency = 1/(2*period_us) = 1000 Hz if period_us = 500
		//if (current_time_ms - beep_start_time_ms <= 400) { // If in first 400ms of the second and still detecting continuity
		if (beeping) {
			buzzer = !buzzer;
			BUZZER_PORT = (buzzer << BUZZER_PIN);
		}
	}
}
void stop_beep() {
	beeping = false;
}
bool beep_for_ms(unsigned long ms, unsigned short frequency) {

	static unsigned long beep_start_time_ms;
	static unsigned long duration_ms;
	if (beeping)
	{
		beep();
		if (millis() - beep_start_time_ms >= duration_ms) {
			stop_beep();
			return false;
		}
		return true;
	}

	duration_ms = ms;
	beep_start_time_ms = millis();
	start_beep(frequency);
	return true;
}

bool beeping_then_pausing = false;
unsigned long beep_then_pause_beep_ms;
unsigned long beep_then_pause_pause_ms;
unsigned short beep_then_pause_freq;
bool beep_then_pause_beeped;
void start_beep_then_pause(unsigned long beep_ms, unsigned long pause_ms, unsigned short frequency) {
	beeping_then_pausing = true;

	beep_then_pause_beep_ms = beep_ms;
	beep_then_pause_pause_ms = pause_ms;
	beep_then_pause_freq = frequency;

	beep_then_pause_beeped = false;
}
bool beep_then_pause() {
	static unsigned long pause_start_ms;

	if (!beeping_then_pausing) {
		return false;
	}
	if (!beep_then_pause_beeped) {
		if (!beep_for_ms(beep_then_pause_beep_ms, beep_then_pause_freq)) {
			beep_then_pause_beeped = true;
			pause_start_ms = millis();
			Serial.println("Finished beeping.");
		}
	} else {
		if (millis() - pause_start_ms >= beep_then_pause_pause_ms) {
			beeping_then_pausing = false;
			Serial.println("Finished pausing.");
		}
	}
	return beeping_then_pausing;
}

short detectContinuity() {
	return !(CONTINUITY_PORT & (1 << CONTINUITY_PIN));
}

short launched(int alt) {
	static int count = 0;
	if (alt - BASE_ALT > ALT_LAUNCHED) {
		count++;
		if (count >= VERIF_SAMPLES) { // need five signals to verify
			return 1; // yay we launched
		}
	} else {
		count = 0;
	}
	return 0;
}

short landed() {
	// for (int i = 0; i < VERIF_SAMPLES; i++) {
	// 	if (altimeter.readAltitudeFt() - BASE_ALT > ALT_LAND) {
	// 		return 0; // if any of the first five signals disagree, return - resend radio signal to retrigger
	// 	}
	// }
	return 1;
}

// Turn on/off 3 LEDs
void setLEDs(uint8_t red, uint8_t green, uint8_t blue) {
    digitalWrite(LED_PIN_RED, red);
    digitalWrite(LED_PIN_GREEN, green);
    digitalWrite(LED_PIN_BLUE, blue);
}

short deploymentSignal() {
	// return !(RECEIVER_PORT & (1 << RECEIVER_PIN));
	return digitalRead(RECEIVER_PIN) == LOW;
}
