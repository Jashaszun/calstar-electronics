#include <avr/io.h>
#include <util/delay.h>
#include <stdint.h>

#define ADC_IO DDRC
#define ADC_PORT PORTC
#define ADC_PIN 1

int main(){
	ADC_setup()
}

//ADMUX operation sets reference voltage to AVcc, enables ADC1, and left-adjusts data registers
//ADCSRA operation enables ADC, enables interrupt flag, scales ADC clock bs 1/128
void ADC_setup(){
	ADMUX = _BV(REFS0)|_BV(MUX0)|_BV(ADLAR);
	ADCSRA = _BV(ADEN)|_BV(ADIE)|0b111;
}

//signals start conversion, waits until ADSC is reset to 0, then returns ADC value.
uint16_t ADC_read(){
	ADCSRA |= _BV(ADSC)
	while( (ADCSRA & _BV(ADSC)) );
	return ADC;
}