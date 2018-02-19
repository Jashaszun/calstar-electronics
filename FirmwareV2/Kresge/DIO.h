#include <avr/io.h>

#define A 0
#define B 1
#define C 2
#define D 3
#define INPUT 0
#define OUTPUT 1
#define LOW
#define HIGH

void pinMode(uint8_t port, uint8_t pin, uint8_t direction);
uint8_t getMode(uint8_t port, uint8_t pin);
uint8_t digitalWrite(uint8_t port, uint8_t pin, uint8_t state);
uint8_t digitalRead(uint8_t port, uint8_t pin)
