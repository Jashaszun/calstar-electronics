#include "Harness.h"
#include "Simulator.h"
#include "SIL.h"

int64_t micros() {
  assert(global_env != NULL);

  return global_env->micros();
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
