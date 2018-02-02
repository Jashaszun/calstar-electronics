// Access control: CalSTAR MR
#include <avr/io.h>
#include <util/delay.h>
#include "SparkFunMPL3115A2.h"

#define F_CPU 16000000

void delay(int ms) {
  int repeats, remainder;
  repeats = ms / 10;
  remainder = ms % 10;
  int i;
  for (i = 0; i < repeats; i++) {
      _delay_ms(10);
  }
  // _delay_ms(remainder);
}

int main() {
  // Set up altimeter
  MPL3115A2 altimeter;
  Wire.begin();
	altimeter.begin();
	altimeter.setModeAltimeter();
	altimeter.setOversampleRate(7);
	altimeter.enableEventFlags();

  float avg = 0;
  while (1) {
    avg = (avg + altimeter.readAltitudeFt()) / 2;
    Serial.println(avg);
    delay(100);
  }
}
