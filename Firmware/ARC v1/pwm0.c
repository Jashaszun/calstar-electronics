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

int angleToWidth(int angle) { // angle between -90 and 90 degrees
    if (angle < 0) {
    	return ((16 * angle) / 180) + 12;
    }
    return ((16 * angle) / 180) + 12;
}

int main(void)
{
    // Port D set as output
    DDRD |= 0xFF;

    OCR0A = angleToWidth(0);
    OCR0B = angleToWidth(90);

    // Set non-inverting phase-corrected
    TCCR0A |= (1 << COM0A1) | (1 << COM0B1) | (1 << WGM00);

    // Set prescaler to 1024
    TCCR0B |= (1 << CS02) | (1 << CS00);

    while(1) {
        OCR0A = angleToWidth(-90);
        OCR0B = angleToWidth(-90);
        delay(1000);
        OCR0A = angleToWidth(90);
        OCR0B = angleToWidth(90);
        delay(1000);
        OCR0A = angleToWidth(-75);
        OCR0B = angleToWidth(-75);
        delay(1000);
        OCR0A = angleToWidth(75);
        OCR0B = angleToWidth(75);
        delay(1000);
        OCR0A = angleToWidth(-60);
        OCR0B = angleToWidth(-60);
        delay(1000);
        OCR0A = angleToWidth(60);
        OCR0B = angleToWidth(60);
        delay(1000);
        OCR0A = angleToWidth(-45);
        OCR0B = angleToWidth(-45);
        delay(1000);
        OCR0A = angleToWidth(45);
        OCR0B = angleToWidth(45);
        delay(1000);
        OCR0A = angleToWidth(-30);
        OCR0B = angleToWidth(-30);
        delay(1000);
        OCR0A = angleToWidth(30);
        OCR0B = angleToWidth(30);
        delay(1000);
        OCR0A = angleToWidth(-15);
        OCR0B = angleToWidth(-15);
        delay(1000);
        OCR0A = angleToWidth(15);
        OCR0B = angleToWidth(15);
        delay(1000);
        OCR0A = angleToWidth(0);
        OCR0B = angleToWidth(0);
        delay(1000);
    }
}
