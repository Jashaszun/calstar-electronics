// Access control: CalSTAR MR

#include <avr/io.h>
#include <util/delay.h>

#define F_CPU 16000000
#define ADC_THRESHOLD 512
#define ADC_IO DDRC
#define ADC_PORT PORTC
#define ADC_PIN 0
#define SOLENOID_IO DDRB
#define SOLENOID_PORT PORTB
#define SOLENOID_PIN 0
#define ACCEL_TOLERANCE 1
#define ALT_THRESHOLD
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
	I2C_init();

	ADC_setup();

	while(ADC_read() < ADC_THRESHOLD);

	// verify sensor data

	SOLENOID_PORT = (1 << SOLENOID_PIN);
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
