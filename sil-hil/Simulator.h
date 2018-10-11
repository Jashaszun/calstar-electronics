#include "common.h"

class Motor {
  string interpolation;
  vector<pair<float, float>> thrust_curve; // <time, force>

public:
  float getForce(float time);
  Motor(string motor_file);
};

class Rocket {
  vec rocket_pos;
  vec rocket_vel;
  vec rocket_dir;  // = rocket_speed / |rocket_speed|?
                   // At the moment yes, but if the simulator gets more advanced then this will not always be the case. -Leo
  Motor motor;

public:
  Rocket(string rocket_file);
};

class Environment {
  Rocket rocket;
  vec wind;

public:
  Environment(Rocket rocket);
  bool done();
  void tick();
  int64_t micros();
};
