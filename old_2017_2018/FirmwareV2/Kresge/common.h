#include <stdint.h>
#include <avr/io.h>
#include <avr/interrupt.h>

#ifndef sbi
#define sbi(sfr, bit) (_SFR_BYTE(sfr) |= _BV(bit))
#endif

#define PI 3.1415926

void init();
