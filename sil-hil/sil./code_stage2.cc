#ifdef SIL
#include "Harness.h"
#else
#include "mbed.h"
#endif

#define LED1 3

DigitalOut myled2(LED1);

void code1() {
  myled2 = 1;
  // wait(0.2);
  myled2 = 0;
  // wait(0.2);
}

#ifndef SIL
int main() {
  while(1) {
    code1();
  }
}
#endif
