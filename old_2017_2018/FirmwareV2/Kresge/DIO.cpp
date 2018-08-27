#include "DIO.h"

uint8_t digitalWrite(uint8_t port, uint8_t pin, uint8_t state) {
  switch (port) {
    case B:
      if (getMode(port, pin) == OUTPUT) {
        PORTB = (state == HIGH ? PORTB | (1 << pin) : PORTB & ~(1 << pin));
        return 1;
      } else {
        return 0;
      }
      break;
    case C:
      if (getMode(port, pin) == OUTPUT) {
        PORTC = (state == HIGH ? PORTC | (1 << pin) : PORTC & ~(1 << pin));
        return 1;
      } else {
        return 0;
      }
      break;
    case D:
      if (getMode(port, pin) == OUTPUT) {
        PORTD = (state == HIGH ? PORTD | (1 << pin) : PORTD & ~(1 << pin));
        return 1;
      } else {
        return 0;
      }
      break;
    default:
      return 0;
      break;
  }
}

uint8_t digitalRead(uint8_t port, uint8_t pin) {
  switch (port) {
    case B:
      return (PINB & (1 << pin)) >> pin;
    case C:
      return (PINC & (1 << pin)) >> pin;
    case D:
      return (PIND & (1 << pin)) >> pin;
    default:
      return 0;
  }
}
