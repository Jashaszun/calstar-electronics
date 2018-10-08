#include "SIL.h"

#define CLOCK_MULTIPLIER (100.0)

chrono::steady_clock::time_point timer;
void start_timer() {
  timer = chrono::high_resolution_clock::now();
}
int64_t elapsed() {
  auto diff = chrono::high_resolution_clock::now() - timer;
  return chrono::duration_cast<chrono::microseconds>(diff).count();
}

int main() {
  Rocket roc("config/rockets/testrocket.config");
  Environment env(roc);

  int64_t code_time = 0; // Time spent in rocket code in microseconds

  start_timer();
  code_init();
  code_time += elapsed();

  while(!env.done()) {
    if (code_time < env.micros()) {
      start_timer();
      code_loop();
      code_time += elapsed();
    } else {
      env.tick();
    }
  }
}
