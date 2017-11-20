#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <stdint.h>

#define ADC_IO DDRC
#define ADC_PORT PORTC
#define ADC_PIN 1
#define	MOTOR_PWM_IO DDRB
#define MOTOR_PWM_PORT PORTB
#define MOTOR1_PWM_PIN 1
#define MOTOR2_PWN_PIN 2
#define MOTOR1_DIR_PORT PORTB
#define MOTOR2_DIR_PORT PORTD
#define MOTOR1_DIR_PIN 0
#define MOTOR2_DIR_PIN 7


int main(){
	DDRB = 0x00
	DDRC = 0x00

	DDRB |= _BV(PB1)
	TCCR1A |= _BV(COM1A1) | _BV(WGM10);
	TCCR1B |= _BV(CS10) | _BV(WGM12);

	uint8_t pwm = 0x00;
	bool up = true;
	for(;;) {

		OCR1A = pwm;

		pwm += up ? 1 : -1;
		if (pwm == 0xff)
			up = false;
		else if (pwm == 0x00)
			up = true;

		_delay_ms(10);

	ADC_setup();
	}
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
	return ADCH;
}