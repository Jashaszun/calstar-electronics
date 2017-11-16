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

int main() {
	/* Pseudocode for deployment sequence:
	1. Wait until ADC input exceeds ADC threshold
	2. Verify vehicle is at rest on the ground with accelerometer/altimeter
	3. Actuate solenoid. */
	DDRC = 0x00; // configure ADC as input
	DDRB = 0xFF; // configure solenoid as output

	ADC_setup();

	while(ADC_read() < ADC_THRESHOLD);
}

void ADC_setup() {
	ADMUX = 01000000; // muxes ADC0, right-adjusts ADC Data Register, and sets AVCC as reference voltage
	ADCSRA = (1 << ADEN) | (1 << ADIE) | 0b111; // Enables ADC and Conversion Complete interrupt flag, and sets prescaler to 128 for an ADC frequency of 16MHz/128 = 125kHz
}

short ADC_read() {
	while(!(ADCSRA & (1 << ADIF))); // wait for interrupt flag
	short result = ADCL | ((ADCH & 0b11) << 8); // get values for ADC data registers
	ADCSRA |= (1 << ADIF); // clear interrupt flag
	return result;
}
