#include "Simulator.h"

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
  // TODO: Initialize motor with file
}

Rocket::Rocket(string rocket_file) : motor("test.json") {
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
