#ifdef SIL
#include "Harness.h"
namespace code2 {
#else
#include "mbed.h"
#endif

void loop() {
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