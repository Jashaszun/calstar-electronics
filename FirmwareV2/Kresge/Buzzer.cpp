#include "Buzzer.h"

Buzzer::Buzzer(uint8_t port, uint8_t pin) {
  _pin = pin;
  _port = port;
  _lastFlip = 0;
  _lastState = 0;
  _lastDuration = 0;
  _active = 0;
  _period = 0;
  pinMode(port, pin, OUTPUT);
}

void Buzzer::_setValuesOnBuzzStart(long duration_ms, short frequency) {
  _period = 1 / (2 * PI * frequency); // T = 1 / (2pi*f)
  _active = 1;
  _lastStart = micros();
  _lastDuration = duration_ms;
}

void Buzzer::blockingBuzz(long duration_ms, short frequency) {
  _setValuesOnBuzzStart(duration_ms, frequency);
  while (micros() - _lastStart > _lastDuration) {
    _flipVoltage();
  }
  stopBuzz();
}

void Buzzer::blockingBuzz(long duration_ms) {
  blockingBuzz(duration_ms, DEFAULT_FREQUENCY);
}

void Buzzer::startBuzz(long duration_ms, short frequency) {
  _setValuesOnBuzzStart(duration_ms, frequency);
}

void Buzzer::startBuzz(long duration_ms) {
  startBuzz(duration_ms, DEFAULT_FREQUENCY);
}

void Buzzer::stopBuzz() {
  _active = 0;
}

bool Buzzer::updateBuzzer() {
  if (millis() - _lastStart > _lastDuration) {
    stopBuzz();
    return 0;
  } else {
    _flipVoltage();
    return 1;
  }
}

bool Buzzer::isBuzzing() {
  return _active;
}

void Buzzer::_flipVoltage() {
  if (_active && millis() - _lastFlip >= _period) {
    _lastState = !_lastState;
    _lastFlip = millis();
    digitalWrite(_port, _pin, _lastState);
  }
}
