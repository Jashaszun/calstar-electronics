#include "DIO.h"

void pinMode(uint8_t port, uint8_t pin, uint8_t direction) {
  switch (port) {
    case B:
      DDRB = (DDRB & ~(1 << pin)) | (direction << pin);
      break;
    case C:
      DDRC = (DDRC & ~(1 << pin)) | (direction << pin);
      break;
    case D:
      DDRD = (DDRD & ~(1 << pin)) | (direction << pin);
      break;
  }
}

uint8_t getMode(uint8_t port, uint8_t pin) {
  switch (port) {
    case B:
      return !!(DDRB & (1 << pin));
    case C:
      return !!(DDRC & (1 << pin));
    case D:
      return !!(DDRD & (1 << pin));
  }
}
