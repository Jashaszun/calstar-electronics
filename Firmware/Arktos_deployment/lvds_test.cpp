// Access control: CalSTAR MR

#include <avr/io.h>
#include <util/delay.h>
#include <Arduino.h>
#include "Wire.h"
#include "SparkFunMPL3115A2.h"

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
#define LED_PIN_GREEN 6
#define LED_PIN_BLUE 7

int main() {
  // Setup
  DDRD = (1 << TRANSMITTER_PIN) | (1 << LED_PIN_RED) | (1 << LED_PIN_GREEN) | (1 << LED_PIN_BLUE); // set outputs and inputs; receiver defaults to input

  // Test
  TRANSMITTER_PORT = (1 << TRANSMITTER_PIN); // transmit to ejection on LVDS
  LED_PORT = (1 << LED_PIN_RED); // indicates no received signal from ejection
  while (!(RECEIVER_PORT & (1 << RECEIVER_PIN))); // wait for signal from ejection
  LED_PORT = (1 << LED_PIN_GREEN) // received signal from ejection; test successful
}
