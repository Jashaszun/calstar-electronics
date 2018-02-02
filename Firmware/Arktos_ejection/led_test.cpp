// Access control: CalSTAR MR

#include <avr/io.h>
#include <util/delay.h>

#define F_CPU 16000000
#define LED_IO DDRC
#define LED_PORT PORTC
#define LED_PIN_RED 3
#define LED_PIN_GREEN 1
#define LED_PIN_BLUE 2

void delay(int ms) {
  for (int i = 0; i < (ms / 10); i++) {
    _delay_ms(10);
  }
  _delay_ms(ms % 10);
}

int main() {
  // Setup
  LED_IO = 0xFF // set LED port to output

  // Test
  while (true) { // cycle through R, G, B for one second each
    LED_PORT = (1 << LED_PIN_RED);
    delay(1000);
    LED_PORT = (1 << LED_PIN_GREEN);
    delay(1000);
    LED_PORT = (1 << LED_PIN_BLUE);
    delay(1000);
  }
}
