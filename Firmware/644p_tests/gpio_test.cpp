#include <avr/io.h>

int main() {
    // DDRA = 0xFF;
    // PORTA = 0xFF;
    // PORTA &= ~(1 << PA7);

    DDRB = 0xFF;
    PORTB = 00000110; //PB2 is DIR (maybe??), PB1 is PWM

    // DDRC = 0xFF;
    // PORTC = 0xFF;

    // DDRD = 0xFF;
    // PORTD = 0xFF;

    // DDRA = 0xFF;
    // PORTA = 0x00;

    // DDRB = 0xFF;
    // PORTB = 0x00;

    // DDRC = 0xFF;
    // PORTC = 0x00;

    // DDRD = 0xFF;
    // PORTD = 0x00;
    while (true) {

    }
}
