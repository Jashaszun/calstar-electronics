#include "Sensors.h"

Accelerometer::Accelerometer(Rocket* roc) : roc(roc) {
  currentError = {0,0,0};
}

// TODO: Add noise/error/direction
vec Accelerometer::getData() {
  return roc->rocket_acc;
}
