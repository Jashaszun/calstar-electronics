#include <avr/io.h>
#include <util/delay.h>

#define F_CPU 16000000
#define ADC_THRESHOLD 512
#define SIGNAL_IO DDRC
#define SIGNAL_PORT PINC
#define SIGNAL_PIN 0
#define SOLENOID_IO DDRB
#define SOLENOID_PORT PORTB
#define SOLENOID_PIN 0
#define LED_IO DDRD
#define LED_PORT PORTD
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
	LED_IO = 0xFF;
	SIGNAL_IO = 0x00;
	SOLENOID_IO = 0xFF;
	while (1) {
		if (SIGNAL_PORT & (1 << SIGNAL_PIN)) {
			LED_PORT = (1 << LED_PIN_GREEN);
			SOLENOID_PORT = 0xFF;
		} else {
			LED_PORT = (1 << LED_PIN_RED);
			SOLENOID_PORT = 0x00;
		}
	}
}