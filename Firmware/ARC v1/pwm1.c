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
    return ((256 * angle) / 180) + 192;
}

int main(void)
{
    // Set outputs
    DDRB |= (1 << DDB1) | (1 << DDB2);

    // Set TOP: frequency = clock / (2 * prescaler * top)
    ICR1 = 2496;

    // Set params - phase correct PWM with top in ICR1, prescaler 1024
    TCCR1A |= (1 << COM1A1) | (1 << COM1B1) | (1 << WGM11);
    TCCR1B |= (1 << WGM13) | (1 << CS11) | (1 << CS10);

    while(1) {
        OCR1A = angleToWidth(-90);
        OCR1B = angleToWidth(-90);
        delay(1000);
        OCR1A = angleToWidth(90);
        OCR1B = angleToWidth(90);
        delay(1000);
        OCR1A = angleToWidth(-75);
        OCR1B = angleToWidth(-75);
        delay(1000);
        OCR1A = angleToWidth(75);
        OCR1B = angleToWidth(75);
        delay(1000);
        OCR1A = angleToWidth(-60);
        OCR1B = angleToWidth(-60);
        delay(1000);
        OCR1A = angleToWidth(60);
        OCR1B = angleToWidth(60);
        delay(1000);
        OCR1A = angleToWidth(-45);
        OCR1B = angleToWidth(-45);
        delay(1000);
        OCR1A = angleToWidth(45);
        OCR1B = angleToWidth(45);
        delay(1000);
        OCR1A = angleToWidth(-30);
        OCR1B = angleToWidth(-30);
        delay(1000);
        OCR1A = angleToWidth(30);
        OCR1B = angleToWidth(30);
        delay(1000);
        OCR1A = angleToWidth(-15);
        OCR1B = angleToWidth(-15);
        delay(1000);
        OCR1A = angleToWidth(15);
        OCR1B = angleToWidth(15);
        delay(1000);
        OCR1A = angleToWidth(0);
        OCR1B = angleToWidth(0);
        delay(1000);
    }
}
