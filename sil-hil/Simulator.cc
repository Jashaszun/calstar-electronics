#include "Simulator.h"
#include "nlohmann/json.hpp"
#include <fstream>
#include <iostream>

using json = nlohmann::json;

float Motor::getForce(float time) {
  // Thrust curve must not be empty
  assert(this->thrust_curve.size() > 0);

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

Motor::Motor(string motor_file) {
  // open file stream
  std::ifstream file(motor_file);

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
}

Rocket::Rocket(string rocket_file) : motor("config/motors/testmotor.json") {
  // TODO: Initialize rocket with file
}

Environment::Environment(Rocket rocket) : rocket(rocket) {

}

bool Environment::done() {
  // TODO: Return whether simulation is done
  return false;
}
void Environment::tick() {
  // TODO: Tick simulation
}

int64_t Environment::micros() {
  // TODO: Return microseconds in environment
  return 0;
}
