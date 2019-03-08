#ifndef SENSORS_H
#define SENSORS_H

#include "common.h"
#include "Simulator.h"

class Accelerometer {
  Rocket* roc;
  vec currentError;

public:
  Accelerometer(Rocket* roc);
  vec getData();
};

#endif
