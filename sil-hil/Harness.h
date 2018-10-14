#include "common.h"

void code_init();
void code_loop();

int64_t micros();

void pinMode(int pin, uint8_t mode);
int digitalRead(int pin);
void digitalWrite(int pin, bool high);
