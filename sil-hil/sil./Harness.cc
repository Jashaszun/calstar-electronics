#include "Harness.h"
#include "Simulator.h"
#include "SIL.h"
#include "common.h"
#include "Sensors.h"

#pragma weak code0
void code0() {}
#pragma weak code1
void code1() {}
#pragma weak code2
void code2() {}
#pragma weak code3
void code3() {}
#pragma weak code4
void code4() {}

int64_t micros() {
  assert(global_env != NULL);

  return global_env->micros();
}

void pinMode(int pin, uint8_t mode) {
  assert(global_env != NULL);
  global_env->pinMode(pin, mode);
}

vec getAcc() {
  static Accelerometer* acc = NULL;
  if (acc == NULL) {
    assert(global_env != NULL);
    acc = new Accelerometer(global_env); // This only ever happens once so memory leak is fine
  }
  return acc->getData();
}

float getAccX() {
  return getAcc().x;
}

float getAccY() {
  return getAcc().y;
}

float getAccZ() {
  return getAcc().z;
}

DigitalOut::DigitalOut(int pin) : pin(pin) { }

void DigitalOut::write(int value) {
  assert(global_env != NULL);

  global_env->setPin(pin, value);
}

int DigitalOut::read() {
  assert(global_env != NULL);

  return global_env->getPin(pin);
}

DigitalOut& DigitalOut::operator=(int value) {
  write(value);
  return *this;
}

DigitalOut::operator int() {
  return read();
}
