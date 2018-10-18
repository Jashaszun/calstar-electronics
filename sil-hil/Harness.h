#include "common.h" // TODO: Maybe take this out? We don't want code "seeing" anything it won't in the real MCU

void code_init();
void code_loop();

int64_t micros();

void pinMode(int pin, uint8_t mode);
int digitalRead(int pin);
void digitalWrite(int pin, bool high);

float getAccX();
float getAccY();
float getAccZ();
