#include "Simulator.h"
#include "nlohmann/json.hpp"
#include <fstream>
#include <iostream>

using json = nlohmann::json;

float Motor::getForce(int time) {
  // if before first entry of thrust curve, return first thrust value
  if (time <= thrust_curve_time[0]) {
    return thrust_curve_force[0];
  }

  // if after last entry, return last thrust value
  if (time >= thrust_curve_time[thrust_curve_len-1]) {
    return thrust_curve_force[thrust_curve_len-1];
  }

  // otherwise, find the first entry of the thrust curve after the given time
  int index = 0;
  while (thrust_curve_time[index] < time) {
    index++;
  }
  
  // interpolate between index-1 and index
  if (interpolation == "linear") {
    // linear interpolation
    float force_0 = thrust_curve_force[index-1];
    float force_1 = thrust_curve_force[index];
    float t_1 = thrust_curve_time[index];
    float t_0 = thrust_curve_time[index-1];
    return force_0 + ((force_1 - force_0)/(t_1-t_0)) * (time-t_0);
  }

  return -1;  // Error if we reach here
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
  int i = 0;

  // NOTE: Assumes thrust_curve_time and thrust_curve_force have enough space for all values.
  for (auto it = motor_json["thrust_curve"].begin(); it != motor_json["thrust_curve"].end(); ++it) {
    float time = stof(it.key());
    float force = it.value();
    thrust_curve_time[i] = time;
    thrust_curve_force[i] = force;
    i++;
  }
  thrust_curve_len = i;
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
