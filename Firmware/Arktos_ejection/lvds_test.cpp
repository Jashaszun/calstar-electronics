// Access control: CalSTAR MR

#include <avr/io.h>
#include <util/delay.h>

#define F_CPU 16000000
#define RECEIVER_IO DDRD
#define RECEIVER_PORT PIND
#define RECEIVER_PIN 7
#define TRANSMITTER_IO DDRB
#define TRANSMITTER_PORT PORTB
#define TRANSMITTER_PIN 0
#define LED_IO DDRC
#define LED_PORT PORTC
#define LED_PIN_RED 3
#define LED_PIN_GREEN 1
#define LED_PIN_BLUE 2

int main() {
  // Setup
  // LVDS receiver defaults to input
  LED_IO = (1 << LED_PIN_RED) | (1 << LED_PIN_GREEN) | (1 << LED_PIN_BLUE); // set LEDs to output
  TRANSMITTER_IO = (1 << TRANSMITTER_PIN); // set LVDS transmitter as output

  // Test
  while(true) {
    if (RECEIVER_PORT & (1 << RECEIVER_PIN)) { // signal from deployment?
      LED_PORT = (1 << TRANSMITTER_PIN) | (1 << LED_PIN_GREEN) // received signal from deployment
      break;
    } else {
      LED_PORT = (1 << TRANSMITTER_PIN) | (1 << LED_PIN_RED); // indicates no received signal from deployment
    }
  }
  TRANSMITTER_PORT = (1 << TRANSMITTER_PIN); // transmit to ejection on LVDS; test complete
}
