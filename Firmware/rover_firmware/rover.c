//Access control: CalSTAR MR

#include <arduino.h>
#include <NewPing.h>
#include <FiniteStateMachine.h>
// #include <avr/io.h>
// #include <avr/interrupt.h>
// #include <util/delay.h>
#include <stdint.h>

//Arduino pin assignments
#define US_1_PING 2
#define SOLAR_SERVO 3
#define US_2_PING 4
#define SKID1_SERVO 5
#define SKID2_SERVO 6
#define MOTOR1_DIR 7
#define MOTOR2_DIR 8
#define MOTOR2_PWM 9
#define MOTOR1_PWM 10
#define ENCODER1_B 12
#define ENCODER1_A 13
#define SOLAR_POT 14
#define SOLAR_ADC 15
#define ENCODER2_B 16
#define ENCODER2_A 17
#define ACCEL_GYRO_SDA 18
#define ACCEL_GYRO_SCL 19

#define SERIAL_BAUD 19200

State Start = State();
State Movement = State();
State SolarDeployment = State();

FSM roverStateMachine = FSM(Start);

void setup(){	
	Serial.begin(SERIAL_BAUD);
	pinMode(MOTOR1_DIR, OUTPUT);
	pinMode(MOTOR1_PWM, OUTPUT);
	pinMode(MOTOR2_DIR, OUTPUT);
	pinMode(MOTOR2_PWM, OUTPUT);
	pinMode(SKID1_SERVO, OUTPUT);
	pinMode(SKID2_SERVO, OUTPUT);
	pinMode(SOLAR_SERVO, OUTPUT);
}

void loop(){
	switch state{
		case 0:
	}
}

void setPWM(int pin, int duty_cycle){
	speed = (duty_cycle / 100) * 255;
	analogWrite(pin, speed);
}

void setMoveDirection(int dir){
	opposite = 1 - dir
	digitalWrite(MOTOR1_DIR, dir);
	digitalWrite(MOTOR2_DIR, opposite);
}

void setup(){

}

/**
#define ADC_IO DDRC
#define ADC_PORT PORT
C#define ADC_PIN 1
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
} **/