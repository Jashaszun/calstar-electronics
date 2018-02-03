// Access control: CalSTAR MR

#include <avr/io.h>
#include <util/delay.h>
#include <Arduino.h>
// #define F_CPU 16000000
#define RECEIVER_IO DDRD
#define RECEIVER_PORT PIND
#define RECEIVER_PIN 7
#define TRANSMITTER_IO DDRB
#define TRANSMITTER_PORT PORTB
#define TRANSMITTER_PIN 8 // 0
#define LED_IO DDRC
#define LED_PORT PORTC
#define LED_PIN_RED 17 // 3
#define LED_PIN_GREEN 15 // 1
#define LED_PIN_BLUE 16 // 2

int main() {
  init();
  Serial.begin(19200);

  pinMode(RECEIVER_PIN, INPUT_PULLUP);
  pinMode(TRANSMITTER_PIN, OUTPUT);
  pinMode(LED_PIN_BLUE, OUTPUT);
  pinMode(LED_PIN_RED, OUTPUT);
  pinMode(LED_PIN_GREEN, OUTPUT);

  digitalWrite(TRANSMITTER_PIN, HIGH);
  while (true) {
    if (Serial.available() > 0) {
      char c = Serial.read();
      if (c == 't') {
        digitalWrite(TRANSMITTER_PIN, HIGH);
      } else if (c == 'f') {
        digitalWrite(TRANSMITTER_PIN, LOW);
      }
      Serial.print("Recieved: ");
      Serial.println(c);
    }
    if (digitalRead(RECEIVER_PIN) == HIGH) {
      digitalWrite(LED_PIN_RED, HIGH);
      digitalWrite(LED_PIN_GREEN, LOW);
      //digitalWrite(TRANSMITTER_PIN, HIGH);
    } else {
      digitalWrite(LED_PIN_RED, LOW);
      digitalWrite(LED_PIN_GREEN, HIGH);
      //digitalWrite(TRANSMITTER_PIN, LOW);
    }
  }
  // digitalWrite(TRANSMITTER_PIN, LOW);
  // Setup
  // LVDS receiver defaults to input
  // LED_IO = (1 << LED_PIN_RED) | (1 << LED_PIN_GREEN) | (1 << LED_PIN_BLUE); // set LEDs to output
  // TRANSMITTER_IO = (1 << TRANSMITTER_PIN); // set LVDS transmitter as output
  // TRANSMITTER_PORT = (1 << TRANSMITTER_PIN); // no signal

  // // Test
  // while(true) {
  //   if (RECEIVER_PORT & (1 << RECEIVER_PIN)) { // signal from deployment?
  //     LED_PORT = (1 << TRANSMITTER_PIN) | (1 << LED_PIN_RED); // high; no signal
  //   } else {
  //     LED_PORT = (1 << TRANSMITTER_PIN) | (1 << LED_PIN_GREEN); // low; signal received
  //     break; // exit so we can transmit back
  //   }
  // }
  // TRANSMITTER_PORT = 0x00; // transmit to ejection on LVDS; test complete
}
