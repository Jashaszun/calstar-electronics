#include "Simulator.h"
#include "SIL.h"
#include "common.h"
#include "nlohmann/json.hpp"
#include <fstream>
#include <iostream>

using json = nlohmann::json;

double Motor::getForce() {
  assert(global_env != NULL);
  // Thrust curve must not be empty
  assert(this->thrust_curve.size() > 0);

  // Return 0 if not started
  if (!activated) {
    return 0;
  }

  double time = (double)(global_env->micros() - start_time) / 1000000.0;

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
    double force_0 = thrust_curve[index - 1].second;
    double force_1 = thrust_curve[index].second;
    double t_1 = thrust_curve[index].first;
    double t_0 = thrust_curve[index - 1].first;
    double slope = (force_1 - force_0) / (t_1 - t_0);
    return force_0 + slope * (time - t_0);
  }

  ERROR();  // Error if we reach here
}

Motor::Motor(string motor_file, string motor_name) : name(motor_name) {
  // open file stream
  ifstream file(motor_file);
  assert(!file.fail());

  // read json from filestream
  json motor_json;
  file >> motor_json;

  // place values into motor
  interpolation = motor_json["interpolation"].get<string>();

  // Load thrust curve. NOTE: Assumes thrust curve is provided in time-increasing order
  for (auto it = motor_json["thrust_curve"].begin(); it != motor_json["thrust_curve"].end(); ++it) {
    double time = stof(it.key());
    double force = it.value();
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

Chute::Chute(double drag, string name) : drag(drag), name(name) {
  activated = false;
}

void Chute::activate() {
  activated = true;
}

double Chute::getDrag() {
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

void Rocket::mapPin(string mapping, bool high, unsigned long val, uint8_t mode, CONNECTION_TYPE ty) {
  for (auto& mcu : microcontrollers) {
    if (mapping.substr(0, mapping.find_first_of(":")) == mcu.name) {
      int pin = stoi(mapping.substr(mapping.find_first_of(":") + 1));
      assert(mcu.pin_map.count(pin) == 0);
      mcu.pin_map[pin] = {ty, high, val, mode};
    }
  }
}

Rocket::Rocket(json rocket_json) {
  DEBUG_OUT << 1 << endl;
  assert(rocket_json.count("weight") == 1);
  rocket_weight = rocket_json["weight"].get<double>();

  assert(rocket_json.count("drag_area") == 1);
  rocket_drag = rocket_json["drag_area"].get<double>();

  assert(rocket_json.count("name") == 1);
  section_name = rocket_json["name"].get<string>();
  DEBUG_OUT << 2 << endl;

  if (rocket_json.count("motors") != 0) {
    for (auto it = rocket_json["motors"].begin(); it != rocket_json["motors"].end(); ++it) {
      motors.emplace_back(it.value()["file"], it.key());
      mapPin(it.value()["pin"], false, motors.size() - 1, PIN_UNDEFINED, CONNECTION_TYPE::MOTOR);
    }
  }
  if (motors.size() == 0) {
    DEBUG_OUT << "WARNING: No motors in section: " << section_name << endl;
  }

  DEBUG_OUT << 3 << endl;
  if (rocket_json.count("chutes") != 0) {
    for (auto it = rocket_json["chutes"].begin(); it != rocket_json["chutes"].end(); ++it) {
      chutes.emplace_back(it.value()["drag_area"], it.key());
      mapPin(it.value()["pin"], false, chutes.size() - 1, PIN_UNDEFINED, CONNECTION_TYPE::CHUTE);
    }
  }
  if (chutes.size() == 0) {
    DEBUG_OUT << "WARNING: No chutes in section: " << section_name << endl;
  }

  DEBUG_OUT << 4 << endl;
  if (rocket_json.count("leds") != 0) {
    for (auto it = rocket_json["leds"].begin(); it != rocket_json["leds"].end(); ++it) {
      leds.emplace_back(it.key());
      mapPin(it.value()["pin"].get<string>(), false, leds.size() - 1, PIN_UNDEFINED, CONNECTION_TYPE::LED);
    }
  }

  DEBUG_OUT << 5 << endl;
}

double Rocket::getDrag() {
  double ret = rocket_drag;

  for (auto chute : chutes) {
    ret += chute.getDrag();
  }

  return ret;
}

Environment::Environment(string sim_file) {
  ifstream file(sim_file);
  if (!file.good()) {
    cerr << "File does not exist: " << sim_file << endl;
    assert(false);
  }

  json sim_json;
  file >> sim_json;
  file.close();

  time = 0;
  groundHeight = sim_json["ground_height"]; // TODO: Make a customizable randomized input
  landed = false;

  // TODO: Make a customizable randomized input
  if (sim_json["wind"]["type"] == "fixed") {
    wind.x = sim_json["wind"]["x"];
    wind.y = sim_json["wind"]["y"];
    wind.z = sim_json["wind"]["z"];
  } else {
    assert(false);
  }

  DEBUG_OUT << "Loading outputs" << endl;
  for (auto out : sim_json["output"]) {
    cerr << out << endl;
    outputs.emplace_back(out);
  }

  DEBUG_OUT << "Loading rocket" << endl;
  ifstream rocket_file(sim_json["rocket"].get<string>());
  assert(!rocket_file.fail());
  json rocket_json;
  rocket_file >> rocket_json;
  vector<shared_ptr<Rocket>> main_section;

  for (json rocket_section : rocket_json) {
    auto r = make_shared<Rocket>(rocket_section);
    DEBUG_OUT << "Loaded section \"" << r->section_name << "\"" << endl;

    r->rocket_pos.x = 0;
    r->rocket_pos.y = 0;
    r->rocket_pos.z = groundHeight;

    r->rocket_vel.x = 0;
    r->rocket_vel.y = 0;
    r->rocket_vel.z = 0;

    r->rocket_acc.x = 0;
    r->rocket_acc.y = 0;
    r->rocket_acc.z = -9.81;

    r->rocket_dir.x = 0;
    r->rocket_dir.y = 0;
    r->rocket_dir.z = 1;

    main_section.push_back(r);
  }
  rocket_sections.push_back(main_section);

  DEBUG_OUT << "Rocket loaded" << endl;
}

bool Environment::done() {
  // TODO: Use timeout
  return landed;
}

void Environment::tick() {
  // TODO: Tick simulation
  time += TICK_MICROS;
  double delta = TICK_MICROS / 1000000.0;

  for (auto section : rocket_sections) {
    if (section.size() == 0) continue;

    vec acc{0, 0, -9.81};
    vec force{0, 0, 0};

    vec old_vel = section[0]->rocket_vel;
    vec old_pos = section[0]->rocket_pos;
    vec old_dir = section[0]->rocket_dir;

    double weight = 0;
    double drag = 0;
    for (auto roc : section) {
      weight += roc->rocket_weight;
      drag = max(drag, roc->getDrag());
    }

    vec effective_wind = wind - old_vel;

    vec wind_force = norm(effective_wind) * (AIR_DENSITY * AIR_DRAG_COEF * drag * pow(mag(effective_wind), 2) / 2.0);
    force = force + wind_force;

    double motor_force = 0;
    for (auto roc : section) {
      for (Motor m : roc->motors) {
        motor_force += m.getForce();
      }
    }
    force = force + old_dir * motor_force;

    acc = acc + force / weight;

    vec new_vel = old_vel + acc * delta;
    vec new_pos = old_pos + new_vel * delta;

    if (new_pos.z < groundHeight) {
      new_pos.z = groundHeight;

      if (new_vel.z < -0.1) {
        double speed = mag(new_vel);
        DEBUG_OUT << "Rocket landed at height " << new_pos.z << " at " << speed << " m/s" << endl;
        landed = true;
      }

      new_vel.x = 0;
      new_vel.y = 0;
      new_vel.z = 0;
    }

    for (auto roc : section) {
      roc->rocket_acc = acc;
      roc->rocket_vel = new_vel;
      roc->rocket_pos = new_pos;
    }

    DEBUG_OUT << "Time: " << time << "  Rocket pos: " << new_pos << "  Acc: " << acc << endl;
  }

}

int64_t Environment::micros() {
  return time;
}

void Environment::setPin(int mcu_id, int pin, bool high) {
  for (auto section : rocket_sections) {
    for (auto roc : section) {
      for (auto mcu : roc->microcontrollers) {
        if (mcu.id == mcu_id) {

          if (mcu.pin_map.count(pin) == 1) {
            auto& pmap = mcu.pin_map[pin];

            if (pmap.mode == PIN_UNDEFINED) {
              DEBUG_OUT << "Warning: Attempting to write to pin " << pin << " before setting pin mode." << endl;
            } else if (pmap.mode == INPUT || pmap.mode == INPUT_PULLUP) {
              DEBUG_OUT << "Warning: Attempting to write to input pin " << pin << endl;
            }

            pmap.high = high;
            if (pmap.type == CONNECTION_TYPE::MOTOR && high == true) {
              roc->motors.at(pmap.index).activate(); // TODO: Maybe set delay on this so super fast writes don't set off the motor
            }
          } else {
            assert(false && "Pin not set up");
          }

          return;
        }
      }
    }
  }
}

int Environment::getPin(int mcu_id, int pin) {
  for (auto section : rocket_sections) {
    for (auto roc : section) {
      for (auto mcu : roc->microcontrollers) {
        if (mcu.id == mcu_id) {
          if (mcu.pin_map.count(pin) == 1) {
            auto& pmap = mcu.pin_map[pin];

            if (pmap.mode == INPUT) {
              return pmap.high;
            } else {
              assert(false && "Invalid pin mode for pin read");
            }
          } else {
            assert(false && "Pin not set up");
          }
        }
      }
    }
  }
  assert(false && "Pin not found");
  ERROR();
}

void Environment::pinMode(int mcu_id, int pin, uint8_t mode) {
  assert(mode == INPUT || mode == OUTPUT || mode == INPUT_PULLUP);

  for (auto section : rocket_sections) {
    for (auto roc : section) {
      for (auto mcu : roc->microcontrollers) {
        if (mcu.id == mcu_id) {
          if (mcu.pin_map.count(pin) == 1) {
            auto& pmap = mcu.pin_map[pin];
            pmap.mode = mode;
          }
        }
      }
    }
  }
}

void Environment::updateOutputs() {
  for (Output& out : outputs) {
    out.update(this);
  }
}

void Environment::finishOutputs() {
  for (Output& out : outputs) {
    out.finish();
  }
}
