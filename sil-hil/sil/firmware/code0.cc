#ifdef SIL
#include "Harness.h"
namespace code0 {
#else
#include "mbed.h"
#endif

#define LED1 10

DigitalOut myled(LED1);
DigitalOut motor(3);

void loop() {
  motor = 1;
  myled = 1;
  // wait(0.2);
  myled = 0;
  // wait(0.2);
}

#ifdef SIL
} // Close namespace
#else
int main() {
  while(1) {
    loop();
  }
}
#endif
