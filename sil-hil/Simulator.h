#ifndef SIMULATOR_H
#define SIMULATOR_H

#include "common.h"

enum class CONNECTION_TYPE {
  MOTOR,
  CHUTE,
  LED,
};

typedef struct {
  CONNECTION_TYPE type;
  bool high;
  unsigned long index;
} pinmapping;

class Motor {
  string name;
  string interpolation;
  bool activated;
  int64_t start_time;
  vector<pair<float, float>> thrust_curve; // <time, force>

public:
  void activate();
  float getForce();
  Motor(string motor_file, string motor_name);
};

class Chute {
  string name;
  bool activated;
  float drag;

public:
  void activate();
  float getDrag();
  Chute(float drag, string name);
};

class LED {
  string name;
  bool activated;

public:
  void set(bool val);
  bool val();
  LED(string name);
};

class Rocket {
public:
  float rocket_weight;
  float rocket_drag;
  vec rocket_pos;  // In meters
  vec rocket_vel;  // In meters / sec
  vec rocket_dir;  // = rocket_speed / |rocket_speed|?
                   // At the moment yes, but if the simulator gets more advanced then this will not always be the case. -Leo
  vector<Motor> motors;
  vector<Chute> chutes;
  vector<LED> leds;
  map<int, pinmapping> pin_map;

  float getDrag();
  Rocket(string rocket_file);
};

class Environment {
  Rocket rocket;
  vec wind;       // In meters / sec
  int64_t time;   // In microseconds
  float groundHeight; // In meters
  bool landed;

public:
  Environment(Rocket r);
  bool done();
  void tick();
  int64_t micros();
  void setPin(int pin, bool high);
};

#endif
