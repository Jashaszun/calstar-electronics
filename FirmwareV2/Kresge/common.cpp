#include "common.h"

void init() {
  sei();
  sbi(TCCR0B, CS01);
	sbi(TCCR0B, CS00);
  sbi(TIMSK0, TOIE0);
}
