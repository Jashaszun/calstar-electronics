// Access control: CalSTAR MR
#include "KString.h"
#include "Buzzer.h"

int main() {
  String teststr("yoyoyo");
  int test;
  test = 1;
  test += 14;
  test *= (int)teststr[2];
  return test;
}

int buzzerTest() {
  Buzzer b(0x00, 1);
  for (int i = 0; i < 1000; i++) {
    b.buzz();
  }
  return 0;
}