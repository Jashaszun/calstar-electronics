#include "SIL.h"
#include <iostream>

chrono::steady_clock::time_point timer;
void start_timer() {
  timer = chrono::high_resolution_clock::now();
}
int64_t elapsed() {
  auto diff = chrono::high_resolution_clock::now() - timer;
  return chrono::duration_cast<chrono::microseconds>(diff).count();
}

Environment* global_env = NULL;
int current_mcu = 0;

void (*loops[MCU_LIMIT])();

int main(int argc, char** argv) {
  if (argc != 2) {
    cerr << "Invalid arguments: ./sil [sim_file.json]" << endl;
    assert(false);
  }

  DEBUG_OUT << "Loading environment..." << endl;
  Environment env{string(argv[1])};
  global_env = &env;
  DEBUG_OUT << "Environment loaded" << endl;

  vector<int64_t> code_time(MCU_LIMIT, 0); // Time spent in rocket code in microseconds

  loops[0] = code0::loop;
  loops[1] = code1::loop;
  loops[2] = code2::loop;
  loops[3] = code3::loop;
  loops[4] = code4::loop;

  // start_timer();
  // code_init();
  // code_time += elapsed();

  DEBUG_OUT << "Starting Simulation" << endl;
  while (!env.done()) {
    bool ran_code = false;
    for (auto& sect : global_env->rocket_sections) {
      for (auto rp : sect) {
        for (auto& mcu : rp->microcontrollers) {
          if (code_time[current_mcu] / CLOCK_MULTIPLIER < env.micros()) {
            current_mcu = mcu.id;
            start_timer();
            loops[current_mcu]();
            code_time[current_mcu] += elapsed() + CODE_OVERHEAD_PENALTY;
            ran_code = true;
          }
        }
      }
    }
    if (!ran_code) {
      env.tick();
    } else {
      // TODO: Maybe put a warning here for loops that run twice without environment ticking
    }
    DEBUG_OUT << env.micros() << endl;
    env.updateOutputs();
  }
  env.finishOutputs();
  global_env = NULL;
}
