#ifndef COMMON_H
#define COMMON_H
#define INPUT ((uint8_t)'I')
#define OUTPUT ((uint8_t)'O')
#define INPUT_PULLUP ((uint8_t)'P')
#define PIN_UNDEFINED ((uint8_t)'U')  // for initial state of the pin
#define LOW ((uint8_t)0)
#define HIGH ((uint8_t)1)

#include <string>
#include <memory>
#include <cmath>
#include <algorithm>
#include <chrono>
#include <vector>
#include <map>
#include <ostream>
#include <stdint.h>

#include "Config.h"

#define DEBUG_OUT cout

using namespace std;

typedef struct {
  double x;
  double y;
  double z;
} vec;

vec operator+(const vec& a, const vec& b);
vec operator-(const vec& a, const vec& b);
vec operator*(const vec& a, const double& b);
vec operator/(const vec& a, const double& b);
vec norm(vec v);
double mag(vec v);
ostream& operator<<(ostream& os, const vec& v);

#define AIR_DRAG_COEF 0.024
#define AIR_DENSITY 0.45

#endif
