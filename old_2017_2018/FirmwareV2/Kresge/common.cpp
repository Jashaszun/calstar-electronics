#include "common.h"

void init() {
  // Set interrupts on
  sei();
  // Set timer to clock / 64
  sbi(TCCR0B, CS01);
	sbi(TCCR0B, CS00);
  // Set interrupt on timer overflow
  sbi(TIMSK0, TOIE0);
}
