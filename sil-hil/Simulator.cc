#include "Simulator.h"
#include "SIL.h"
#include "nlohmann/json.hpp"
#include <fstream>
#include <iostream>

using json = nlohmann::json;

float Motor::getForce() {
  assert(global_env != NULL);
  // Thrust curve must not be empty
  assert(this->thrust_curve.size() > 0);

  // Return 0 if not started
  if (!activated) {
    return 0;
  }

  float time = (float)(global_env->micros() - start_time) / 1000000.0;

  // if before first entry of thrust curve, return first thrust value
  if (time <= thrust_curve[0].first) {
    return thrust_curve[0].second;
  }

  // if after last entry, return last thrust value
  if (time >= thrust_curve[thrust_curve.size() - 1].first) {
    return thrust_curve[thrust_curve.size() - 1].second;
  }

  // otherwise, find the first entry of the thrust curve after the given time
  int index = 0;
  while (thrust_curve[index].first < time) {
    index++;
  }

  // If it's on a point then return the point
  // if (abs(thrust_curve[index].first - time) {
  //   return thrust_curve[index].second;
  // }

  // interpolate between index-1 and index
  if (interpolation == "linear") {
    // linear interpolation
    float force_0 = thrust_curve[index - 1].second;
    float force_1 = thrust_curve[index].second;
    float t_1 = thrust_curve[index].first;
    float t_0 = thrust_curve[index - 1].first;
    float slope = (force_1 - force_0) / (t_1 - t_0);
    return force_0 + slope * (time - t_0);
  }

  assert(false);  // Error if we reach here
}

Motor::Motor(string motor_file, string motor_name) : name(motor_name) {
  // open file stream
  ifstream file(motor_file);
  assert(!file.fail());

  // read json from filestream
  json motor_json;
  file >> motor_json;

  // place values into motor
  interpolation = motor_json["interpolation"];

  // Load thrust curve. NOTE: Assumes thrust curve is provided in time-increasing order
  for (auto it = motor_json["thrust_curve"].begin(); it != motor_json["thrust_curve"].end(); ++it) {
    float time = stof(it.key());
    float force = it.value();
    thrust_curve.push_back(make_pair(time, force));
  }

  activated = false;
}

void Motor::activate() {
  assert(global_env != NULL);

  if (!activated) {
    activated = true;
    start_time = global_env->micros();
  }
}

Chute::Chute(float drag, string name) : drag(drag), name(name) {
  activated = false;
}

void Chute::activate() {
  activated = true;
}

float Chute::getDrag() {
  if (activated) {
    return drag;
  }
  return 0;
}

LED::LED(string name) : name(name) {
  activated = false;
}

bool LED::val() {
  return activated;
}

void LED::set(bool val) {
  activated = val;
}

Rocket::Rocket(string rocket_file) {
  // TODO: Initialize rocket with file
  ifstream file(rocket_file);
  assert(!file.fail());

  json rocket_json;
  file >> rocket_json;

  assert(rocket_json.count("weight") == 1);
  rocket_weight = rocket_json["weight"];

  assert(rocket_json.count("drag_area") == 1);
  rocket_drag = rocket_json["drag_area"];

  if (rocket_json.count("motors") != 0) {
    for (auto it = rocket_json["motors"].begin(); it != rocket_json["motors"].end(); ++it) {
      motors.emplace_back(it.value()["file"], it.key());
      int pin = it.value()["pin"];
      assert(pin_map.count(pin) == 0);
      pin_map[pin] = {CONNECTION_TYPE::MOTOR, false, motors.size() - 1};
    }
  }
  if (motors.size() == 0) {
    DEBUG_OUT << "WARNING: No motors" << endl;
  }

  if (rocket_json.count("chutes") != 0) {
    for (auto it = rocket_json["chutes"].begin(); it != rocket_json["chutes"].end(); ++it) {
      chutes.emplace_back(it.value()["drag_area"], it.key());
      int pin = it.value()["pin"];
      assert(pin_map.count(pin) == 0);
      pin_map[pin] = {CONNECTION_TYPE::CHUTE, false, chutes.size() - 1};
    }
  }
  if (chutes.size() == 0) {
    DEBUG_OUT << "WARNING: No chutes" << endl;
  }

  if (rocket_json.count("leds") != 0) {
    for (auto it = rocket_json["leds"].begin(); it != rocket_json["leds"].end(); ++it) {
      leds.emplace_back(it.key());
      int pin = it.value();
      assert(pin_map.count(pin) == 0);
      pin_map[pin] = {CONNECTION_TYPE::LED, false, leds.size() - 1};
    }
  }
}

float Rocket::getDrag() {
  float ret = rocket_drag;

  for (auto chute : chutes) {
    ret += chute.getDrag();
  }

  return ret;
}

Environment::Environment(Rocket r) : rocket(r) {
  time = 0;
  groundHeight = 10; // TODO: Make a customizable randomized input
  landed = false;

  wind.x = 0; // TODO: Make a customizable randomized input
  wind.y = 0;
  wind.z = 0;

  rocket.rocket_pos.x = 0;
  rocket.rocket_pos.y = 0;
  rocket.rocket_pos.z = groundHeight;

  rocket.rocket_vel.x = 0;
  rocket.rocket_vel.y = 0;
  rocket.rocket_vel.z = 0;

  rocket.rocket_dir.x = 0;
  rocket.rocket_dir.y = 0;
  rocket.rocket_dir.z = 1;

}

bool Environment::done() {
  // TODO: Return whether simulation is done
  return landed;
}

void Environment::tick() {
  // TODO: Tick simulation
  time += TICK_MICROS;
  double delta = TICK_MICROS / 1000000.0;

  vec acc{0, 0, -9.81};
  vec force{0, 0, 0};

  vec effective_wind = wind - rocket.rocket_vel;
  vec wind_force = norm(effective_wind) * (AIR_DENSITY * AIR_DRAG_COEF * rocket.getDrag() * pow(mag(effective_wind), 2) / 2.0);
  force = force + wind_force;

  double motor_force = 0;
  for (Motor m : rocket.motors) {
    motor_force += m.getForce();
  }
  force = force + rocket.rocket_dir * motor_force;

  acc = acc + force / rocket.rocket_weight;

  rocket.rocket_vel = rocket.rocket_vel + acc * delta;

  rocket.rocket_pos = rocket.rocket_pos + rocket.rocket_vel * delta;

  if (rocket.rocket_pos.z < groundHeight) {
    rocket.rocket_pos.z = groundHeight;

    if (rocket.rocket_vel.z < -0.1) {
      float speed = mag(rocket.rocket_vel);
      DEBUG_OUT << "Rocket landed at height " << rocket.rocket_pos.z << " at " << speed << " m/s" << endl;
      landed = true;
    }

    rocket.rocket_vel.x = 0;
    rocket.rocket_vel.y = 0;
    rocket.rocket_vel.z = 0;
  }

  DEBUG_OUT << "Time: " << time << "  Rocket pos: " << rocket.rocket_pos << "  Acc: " << acc << endl;
}

int64_t Environment::micros() {
  // TODO: Return microseconds in environment
  return time;
}

void Environment::setPin(int pin, bool high) {
  if (rocket.pin_map.count(pin) == 1) {
    auto& pmap = rocket.pin_map[pin];
    pmap.high = high;
    if (pmap.type == CONNECTION_TYPE::MOTOR && high == true) {
      rocket.motors.at(pmap.index).activate(); // TODO: Maybe set delay on this so super fast writes don't set off the motor
    }
  }
}
