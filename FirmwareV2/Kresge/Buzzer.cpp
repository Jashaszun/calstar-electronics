#include "Buzzer.h"

Buzzer::Buzzer(uint8_t port, uint8_t pin) {
  _port = port;
  _pin = pin;
  _lastFlip = 0;
  _lastState = LOW;
  pinMode(port, pin, OUTPUT);
}

Buzzer::buzz() {
  _lastState = !_lastState;
  long ts = millis();
  // Flip every millis
  if (ts - _lastFlip < 1) {
    digitalWrite(_port, _pin, _lastState);
    _lastFlip = ts;
  }
}