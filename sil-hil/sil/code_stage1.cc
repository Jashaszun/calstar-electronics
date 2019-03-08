#ifdef SIL
#include "Harness.h"
#else
#include "mbed.h"
#endif

#define LED1 3

DigitalOut myled(LED1);

void code0() {
  myled = 1;
  // wait(0.2);
  myled = 0;
  // wait(0.2);
}

#ifndef SIL
int main() {
  while(1) {
    code0();
  }
}
#endif
