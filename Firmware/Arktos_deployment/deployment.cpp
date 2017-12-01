// Access control: CalSTAR MR

#include <avr/io.h>
#include <util/delay.h>
#include "Wire.h"
#include "SparkFunMPL3115A2.h"

#define F_CPU 16000000
#define ADC_THRESHOLD 512
#define ADC_IO DDRC
#define ADC_PORT PORTC
#define ADC_PIN 0
#define SOLENOID_IO DDRB
#define SOLENOID_PORT PORTB
#define SOLENOID_PIN 0
#define LED_IO DDRD
#define LED_PORT PORTB
#define LED_PIN_RED 5
#define LED_PIN_GREEN 6
#define LED_PIN_BLUE 7
#define ACCEL_TOLERANCE 1
#define ALT_LAND 50
#define ALT_LAUNCHED 200
#define ACCEL_ADDR 0x00
#define ALT_ADDR 0x60
#define ALT_WRITE_ADDR 0xC0
#define ALT_READ_ADDR 0xC1

int main() {
	/* Pseudocode for deployment sequence:
	1. Verify vehicle has launched with altimeter
	2. Wait until ADC input exceeds ADC threshold
	3. Verify vehicle is at rest on the ground with altimeter
	4. Actuate solenoid. */

	// Setup
	ADC_IO = 0x00; // configure ADC as input
	SOLENOID_IO = 0xFF; // configure solenoid as output
	LED_IO = 0xFF; // configure LED as output
	SOLENOID_PORT = 0x00;
	LED_PORT = 0x00;
	MPL3115A2 altimeter;

	ADC_setup();
	Wire.begin();
	altimeter.begin();
	altimeter.setModeAltimeter();
	altimeter.setOversampleRate(7);
	altimeter.enableEventFlags();

	float avg = altimeter.readAltitudeFt();

	// Main program thread
	while (altimeter.readAltitudeFt() < ALT_LAUNCHED - avg) { // wait for vehicle to launch 
		avg = (avg + altimeter.readAltitudeFt()) / 2; // compute moving exponential average
	}
	LED_PORT = (1 << LED_PIN_RED); // set LED to red to indicate launch
	while (waitForSignal() == 0); // wait for ejection signal and cross-check with sensor
	LED_PORT = (1 << LED_PIN_GREEN); // set LED to green to indicate receipt of signal

	SOLENOID_PORT = (1 << SOLENOID_PIN); // trigger solenoid
	LED_PORT = (1 << LED_PIN_BLUE) // set LED to blue to indicate completion of program
	return 0;
}

void ADC_setup() {
	ADMUX = (1 << REFS0) | (1 << ADLAR); // muxes ADC0, left-adjusts ADC Data Register, and sets AVCC as reference voltage
	ADCSRA = (1 << ADEN) | (1 << ADIE) | 0b111; // Enables ADC and Conversion Complete interrupt flag, and sets prescaler to 128 for an ADC frequency of 16MHz/128 = 125kHz
}

char ADC_read() {
	ADCSRA |= (1 << ADSC);
	while(!(ADCSRA & (1 << ADIF))); // wait for interrupt flag
	char result = ADCH; // get values for ADC data registers
	ADCSRA |= (1 << ADIF); // clear interrupt flag
	return result;
}

char waitForSignal() {
	while(ADC_read() < ADC_THRESHOLD); // wait for ADC value
	if (altimeter.readAltitudeFt() > ALT_LAND) { // if not on ground
		return 0;
	}
	return 1;
}
