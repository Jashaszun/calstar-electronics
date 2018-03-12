// Access control: CalSTAR MR
#include "KString.h"
#include "Buzzer.h"
#include "common.h"
// #include "KString.h"
#include "DIO.h"
#include "Timer.h"
// #include <util/delay.h>

#define LEDREDPORTPIN C, 3
#define LEDGREENPORTPIN C, 1
#define LEDBLUEPORTPIN C, 2

int main() {
  init();
  // String teststr("yoyoyo");
  // int test;
  // test = 1;
  // test += 14;
  // test *= (int)teststr[2];
  // cli();
  // return test;
  pinMode(LEDREDPORTPIN, OUTPUT);
  pinMode(LEDGREENPORTPIN, OUTPUT);
  pinMode(LEDBLUEPORTPIN, OUTPUT);
  while (true) {
    int stage = (millis() >> 7) & 7;
    digitalWrite(LEDREDPORTPIN, !!(stage & 1));
    digitalWrite(LEDGREENPORTPIN, !!(stage & 2));
    digitalWrite(LEDBLUEPORTPIN, !!(stage & 4));
  }
  // while (true) {}
}

int buzzerTest() {
  AutoBuzzer b(B, 1);
  b.startBuzz(1000); // 1 second
  while (b.update()) { }
  return 0;
}
