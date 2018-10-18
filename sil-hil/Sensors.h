#ifndef SENSORS_H
#define SENSORS_H

#include "common.h"
#include "Simulator.h"

class Accelerometer {
  Environment* env;
  vec currentError;
  
public:
  Accelerometer(Environment* env);
  vec getData();
};

#endif
