#include "Simulator.h"
#include "Output.h"
#include "Sensors.h"
#include "Harness.h"
#include <fstream>

Output::Output(json config) {
  file = config["file"];
  frequency = config["frequency"];
  lastPoll = 0;
  string ty = config["type"];
  if (ty == "acceleration") {
    type = OUTPUTTYPE::ACCELERATION;
  } else if (ty == "acceleration") {
    type = OUTPUTTYPE::ACCELERATION;
  } else if (ty == "accelerometer") {
    type = OUTPUTTYPE::ACCELEROMETER;
  } else if (ty == "altitude") {
    type = OUTPUTTYPE::ALTITUDE;
  } else if (ty == "altimeter") {
    type = OUTPUTTYPE::ALTIMETER;
  } else {
    assert(false);
  }
}

void Output::update(Environment* env) {
  if (env->micros() >= lastPoll + frequency) {
    lastPoll = env->micros();

    csv << env->micros();
    csv << ", ";

    switch (type) {
    case OUTPUTTYPE::ACCELERATION:
      {
        vec acc = env->rocket.rocket_acc;
        csv << acc.x;
        csv << ", ";
        csv << acc.y;
        csv << ", ";
        csv << acc.z;
      }
      break;
    case OUTPUTTYPE::ACCELEROMETER:
      {
        csv << getAccX();
        csv << ", ";
        csv << getAccY();
        csv << ", ";
        csv << getAccZ();
      }
      break;
    case OUTPUTTYPE::ALTITUDE:
      {
        csv << env->rocket.rocket_pos.z;
      }
      break;
    default:
      assert(false);
    };

    csv << "\n";
  }
}

void Output::finish() {
  while (ifstream(file).good()) file = "_" + file; // Ensure unique filename
  ofstream out(file);
  out << csv.str();
  out.close();
}
