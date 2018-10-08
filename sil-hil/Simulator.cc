#include "Simulator.h"

float Motor::getForce(int time) {
  // TODO: Get force based off curve
  return 0;
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
