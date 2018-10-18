#include "Sensors.h"

Accelerometer::Accelerometer(Environment* env) : env(env) {
  currentError = {0,0,0};
}

// TODO: Add noise/error/direction
vec Accelerometer::getData() {
  return env->rocket.rocket_acc;
}
