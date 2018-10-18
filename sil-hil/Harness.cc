#include "Harness.h"
#include "Simulator.h"
#include "SIL.h"
#include "common.h"
#include "Sensors.h"

int64_t micros() {
  assert(global_env != NULL);

  return global_env->micros();
}

void pinMode(int pin, uint8_t mode) {
  assert(global_env != NULL);
  global_env->pinMode(pin, mode);
}

int digitalRead(int pin) {
  assert(global_env != NULL);
  // TODO: Read from environment
  return 0;
}

void digitalWrite(int pin, bool high) {
  assert(global_env != NULL);
  // TODO: Interact with environment
  global_env->setPin(pin, high);
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
