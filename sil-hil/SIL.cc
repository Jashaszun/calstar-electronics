#include "SIL.h"

chrono::steady_clock::time_point timer;
void start_timer() {
  timer = chrono::high_resolution_clock::now();
}
int64_t elapsed() {
  auto diff = chrono::high_resolution_clock::now() - timer;
  return chrono::duration_cast<chrono::microseconds>(diff).count();
}

Environment* global_env = NULL;

int main() {
  Rocket roc("config/rockets/testrocket.json");
  Environment env(roc);
  global_env = &env;

  int64_t code_time = 0; // Time spent in rocket code in microseconds

  start_timer();
  code_init();
  code_time += elapsed();

  while (!env.done()) {
    if (code_time / CLOCK_MULTIPLIER < env.micros()) {
      start_timer();
      code_loop();
      code_time += elapsed() + 100; // TODO: Figure out legit overhead penalty
    } else {
      env.tick();
    }
  }
  global_env = NULL;
}
