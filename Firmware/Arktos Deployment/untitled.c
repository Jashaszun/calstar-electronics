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

int main() {
	DDRD = 0xFF;
	DDRB = 0xFF;
	PORTB = 0x00;
	PORTD = (1 << 5);
	delay(1000);
	PORTB = 0xFF;
	PORTD = (1 << 6);
	delay(2000);
	PORTB = 0x00;
	PORTD = (1 << 7);
}