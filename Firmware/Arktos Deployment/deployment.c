// Access control: CalSTAR MR

#include <avr/io.h>
#include <util/delay.h>
#include "Wire.h"
#include "SparkFunMPL3115A2"

#define F_CPU 16000000
#define ADC_THRESHOLD 512
#define ADC_IO DDRC
#define ADC_PORT PORTC
#define ADC_PIN 0
#define SOLENOID_IO DDRB
#define SOLENOID_PORT PORTB
#define SOLENOID_PIN 0
#define ACCEL_TOLERANCE 1
#define ALT_THRESHOLD 50
#define ACCEL_ADDR 0x00
#define ALT_ADDR 0x60
#define ALT_WRITE_ADDR 0xC0
#define ALT_READ_ADDR 0xC1

int main() {
	/* Pseudocode for deployment sequence:
	1. Wait until ADC input exceeds ADC threshold
	2. Verify vehicle is at rest on the ground with accelerometer/altimeter
	3. Actuate solenoid. */

	// Setup
	ADC_IO = 0x00; // configure ADC as input
	SOLENOID_IO = 0xFF; // configure solenoid as output
	SOLENOID_PORT = 0x00;
	MPL3115A2 altimeter;

	ADC_setup();
	Wire.begin();
	altimeter.begin();
	altimeter.setModeAltimeter();
	altimeter.setOversampleRate(7);
	altimeter.enableEventFlags();

	// Main program thread
	waitForSignal(); // wait for ejection signal and cross-check with sensor

	SOLENOID_PORT = (1 << SOLENOID_PIN); // trigger solenoid
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

void waitForSignal() {
	while(ADC_read() < ADC_THRESHOLD); // wait for ADC value
	if (altimeter.readAltitudeFt() > ALT_THRESHOLD) { // if not on ground
		waitForSignal(); // recursively call self, not returning until both conditions are met simultaneously
	}
	return;
}
