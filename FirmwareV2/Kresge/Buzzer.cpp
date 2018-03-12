#include "common.h"
#include "Buzzer.h"
#include "AIO.h"

ManualBuzzer::ManualBuzzer(uint8_t port, uint8_t pin) {
  _pin = pin;
  _port = port;
  _lastFlip = 0;
  _lastState = 0;
  _lastDuration = 0;
  _active = 0;
  _period = 0;
  pinMode(port, pin, OUTPUT);
}

void ManualBuzzer::_setValuesOnBuzzStart(long duration_ms, short frequency) {
  _period = 1 / (2 * PI * frequency); // T = 1 / (2pi*f)
  _active = 1;
  _lastStart = micros();
  _lastDuration = duration_ms;
}

void ManualBuzzer::blockingBuzz(long duration_ms, short frequency) {
  _setValuesOnBuzzStart(duration_ms, frequency);
  while (micros() - _lastStart > _lastDuration) {
    _flipVoltage();
  }
  stopBuzz();
}

void ManualBuzzer::blockingBuzz(long duration_ms) {
  blockingBuzz(duration_ms, DEFAULT_FREQUENCY);
}

void ManualBuzzer::startBuzz(long duration_ms, short frequency) {
  _setValuesOnBuzzStart(duration_ms, frequency);
}

void ManualBuzzer::startBuzz(long duration_ms) {
  startBuzz(duration_ms, DEFAULT_FREQUENCY);
}

void ManualBuzzer::stopBuzz() {
  _active = 0;
}

bool ManualBuzzer::updateBuzzer() {
  if (millis() - _lastStart > _lastDuration) {
    stopBuzz();
    return 0;
  } else {
    _flipVoltage();
    return 1;
  }
}

bool ManualBuzzer::isBuzzing() {
  return _active;
}

void ManualBuzzer::_flipVoltage() {
  if (_active && millis() - _lastFlip >= _period) {
    _lastState = !_lastState;
    _lastFlip = millis();
    digitalWrite(_port, _pin, _lastState);
  }
}



AutoBuzzer::AutoBuzzer(uint8_t port, uint8_t pin) {
  _pin = pin;
  _port = port;
  _active = false;
  pinMode(port, pin, OUTPUT);
}

void AutoBuzzer::startBuzz(long duration_ms) {
  _lastStart = millis();
  _duration = duration_ms;
  _active = true;
  analogWrite(_port, _pin, 127);
}

bool AutoBuzzer::update() {
  if (_active && millis() > _lastStart + _duration) {
    this->stopBuzz();
  }
  return _active;
}

void AutoBuzzer::stopBuzz() {
  analogWrite(_port, _pin, 0);
  _active = false;
}
