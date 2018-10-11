#include "common.h"

class Motor {
public:
  string interpolation;
  int thrust_curve_len;
  float thrust_curve_time[4];
  float thrust_curve_force[4];
  float getForce(int time);
  Motor(string motor_file);
};

class Rocket {
  vec rocket_pos;
  vec rocket_speed; // velocity?
  vec rocket_dir;  // = rocket_speed / |rocket_speed|?
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
