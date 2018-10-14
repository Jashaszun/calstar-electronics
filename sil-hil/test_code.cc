#include "Harness.h"
#include "common.h"

#define MOTOR_PIN 3

void code_init() {
  // TODO: Put code here
  pinMode(MOTOR_PIN, OUTPUT);
  digitalWrite(MOTOR_PIN, true);
}

void code_loop() {
  // TODO: Put code here
}
