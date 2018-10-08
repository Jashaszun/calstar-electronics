#include "common.h"

class Motor {
public:
  float getForce(int time);
  Motor(string motor_file);
};

class Rocket {
  vec rocket_pos;
  vec rocket_speed;
  vec rocket_dir;
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
