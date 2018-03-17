#include <avr/io.h>
#include <util/delay.h>

#define F_CPU 16000000

#define DDR_US1Trig DDRC
#define PORT_US1Trig PORTC
#define MASK_US1Trig (1 << 4)
#define DDR_US2Trig DDRC
#define PORT_US2Trig PORTC
#define MASK_US2Trig (1 << 3)
#define DDR_US1Echo DDRC
#define PIN_US1Echo PINC
#define MASK_US1Echo (1 << 5)
#define DDR_US2Echo DDRC
#define PIN_US2Echo PINC
#define MASK_US2Echo (1 << 2)

#define CENTIMETER 1
#define INCH 0

void setup() {
  DDRC = MASK_US1Trig | MASK_US2Trig;
}

int main() {
  PORT_US1Trig &= ~(MASK_US1Trig); // set trig pin low
  PORT_US2Trig &= ~(MASK_US2Trig); // set trig pin low
  _delay_ms(10);
  test_us1();
}

unsigned int test_us1(unsigned short units) {
  unsigned int microseconds = 0;
  PORT_US1Trig |= MASK_US1Trig; // set trig pin high
  _delay_us(10);
  PORT_US1Trig &= ~(MASK_US1Trig); // set trig pin low
  while (!(PIN_US1Echo & MASK_US1Echo));
  while (PIN_US1Echo & MASK_US1Echo) {
    microseconds += 1;
    _delay_us(1);
  }
  if (units == CENTIMETER) {
    return microseconds / 58;
  } else if (units == INCH) {
    return microseconds / 148;
  } else {
    return 0;
  }
}

unsigned int test_us2(unsigned short units) {
  unsigned int microseconds = 0;
  PORT_US2Trig |= MASK_US2Trig; // set trig pin high
  _delay_us(10);
  PORT_US2Trig &= ~(MASK_US2Trig); // set trig pin low
  while (!(PIN_US2Echo & MASK_US2Echo));
  while (PIN_US2Echo & MASK_US2Echo) {
    microseconds += 1;
    _delay_us(1);
  }
  if (units == CENTIMETER) {
    return microseconds / 58;
  } else if (units == INCH) {
    return microseconds / 148;
  } else {
    return 0;
  }
}
