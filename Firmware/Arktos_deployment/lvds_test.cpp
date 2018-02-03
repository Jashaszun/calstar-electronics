// Access control: CalSTAR MR

#include <avr/io.h>
#include <util/delay.h>
#include <Arduino.h>

#define F_CPU 16000000
#define RECEIVER_IO DDRD
#define RECEIVER_PORT PIND
#define RECEIVER_PIN 3
#define TRANSMITTER_IO DDRD
#define TRANSMITTER_PORT PORTD
#define TRANSMITTER_PIN 2
#define LED_IO DDRD
#define LED_PORT PORTD
#define LED_PIN_RED 5
#define LED_PIN_GREEN 7
#define LED_PIN_BLUE 6

int main() {

  pinMode(RECEIVER_PIN, INPUT);
  pinMode(TRANSMITTER_PIN, OUTPUT);
  pinMode(LED_PIN_BLUE, OUTPUT);
  pinMode(LED_PIN_GREEN, OUTPUT);
  pinMode(LED_PIN_RED, OUTPUT);

  // digitalWrite(LED_PIN_BLUE, HIGH);

  // digitalWrite(TRANSMITTER_PIN, HIGH);
  // delay(1000);
  digitalWrite(TRANSMITTER_PIN, LOW);
  while(true) {
    if (digitalRead(RECEIVER_PIN) == HIGH) {
      digitalWrite(LED_PIN_RED, HIGH);
      digitalWrite(LED_PIN_GREEN, LOW);
    } else {
      digitalWrite(LED_PIN_RED, LOW);
      digitalWrite(LED_PIN_GREEN, HIGH);
    }
  }
  // Setup
  // LVDS receiver defaults to input
  // LED_IO = (1 << LED_PIN_RED) | (1 << LED_PIN_GREEN) | (1 << LED_PIN_BLUE); // set LEDs to output
  // TRANSMITTER_IO |= (1 << TRANSMITTER_PIN); // set LVDS transmitter as output

  // // Test
  // TRANSMITTER_PORT |= (1 << TRANSMITTER_PIN); // high (no signal), should be red
  // delay(1000); // wait 1 second
  // TRANSMITTER_PORT &= ~(1 << TRANSMITTER_PIN); // transmit to ejection on LVDS
  // while(true) {
  //   if (RECEIVER_PORT & (1 << RECEIVER_PIN)) { // signal from ejection?
  //     LED_PORT &= ~(1 << LED_PIN_GREEN);
  //     LED_PORT |= (1 << LED_PIN_RED); // high; indicates no received signal from ejection
  //   } else {
  //     LED_PORT &= ~(1 << LED_PIN_RED);
  //     LED_PORT |= (1 << LED_PIN_GREEN); // low; received signal from ejection; test successful
  //   }
  // }
}
