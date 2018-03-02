#include "Buzzer.h"

// TODO replace all us with ms?

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

void Buzzer::_setValuesOnBuzzStart(long duration_us, short frequency) {
  _period = 1 / (2 * PI * frequency); // T = 1 / (2pi*f)
  _active = 1;
  _lastStart = micros();
  _lastDuration = duration_us;
}

void Buzzer::blockingBuzz(long duration_us, short frequency) {
  _setValuesOnBuzzStart(duration_us, frequency);
  while (micros() - _lastStart > _lastDuration) {
    _flipVoltage();
  }
  stopBuzz();
}

void Buzzer::blockingBuzz(long duration_us) {
  blockingBuzz(duration_us, DEFAULT_FREQUENCY);
}

void Buzzer::startBuzz(long duration_us, short frequency) {
  _setValuesOnBuzzStart(duration_us, frequency);
}

void Buzzer::startBuzz(long duration_us) {
  startBuzz(duration_us, DEFAULT_FREQUENCY);
}

void Buzzer::stopBuzz() {
  _active = 0;
}

bool Buzzer::updateBuzzer() {
  if (micros() - _lastStart > _lastDuration) {
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
  if (_active && micros() - _lastFlip >= _period) {
    _lastState = !_lastState;
    _lastFlip = micros();
    digitalWrite(_port, _pin, _lastState);
  }
}
