#define F_CPU 16000000
#include <avr/io.h>
#include <util/delay.h>

void delay(int ms) {
    int repeats, remainder;
    repeats = ms / 10;
    remainder = ms % 10;
    int i;
    for (i = 0; i < repeats; i++) {
        _delay_ms(10);
    }
    _delay_ms(remainder);
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

int main() {
	DDRD = 0xFF;
	PORTD = (1 << 5);
	ADC_setup();
	while (ADC_read() < 512);
	PORTB = (1 << 6);
}