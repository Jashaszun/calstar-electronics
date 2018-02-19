#include <avr/io.h>

#define B 0
#define C 1
#define D 2
#define INPUT 0
#define OUTPUT 1
#define LOW 0
#define HIGH 1

void pinMode(uint8_t port, uint8_t pin, uint8_t direction);
uint8_t getMode(uint8_t port, uint8_t pin);
uint8_t digitalWrite(uint8_t port, uint8_t pin, uint8_t state);
uint8_t digitalRead(uint8_t port, uint8_t pin);
