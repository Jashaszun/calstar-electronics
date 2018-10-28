#ifndef HARNESS_H
#define HARNESS_H

#include "common.h" // TODO: Maybe take this out? We don't want code "seeing" anything it won't in the real MCU

extern int current_mcu;

void code0();
void code1();
void code2();
void code3();
void code4();

int64_t micros();

void pinMode(int pin, uint8_t mode);
int digitalRead(int pin);
void digitalWrite(int pin, bool high);

float getAccX();
float getAccY();
float getAccZ();

class DigitalOut {
  int pin;
public:
  DigitalOut(int pin);
  void write(int value);
  int read();
  DigitalOut& operator= (int value);
  operator int();
};

#endif
