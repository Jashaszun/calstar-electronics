#include "common.h"
#include "Simulator.h"
#include "Harness.h"

extern Environment* global_env;

class MCU {
  int ind;
  uint64_t time_spent;
  void (*func)();
};
