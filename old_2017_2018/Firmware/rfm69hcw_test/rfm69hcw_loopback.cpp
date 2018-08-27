/*#include "rfm69hcw.h"*/
/*#include "UART.c"*/
/*#define USART_BAUDRATE 19200
#ifndef F_CPU
#define F_CPU 16000000UL
#endif
#define UBRR_VALUE (((F_CPU / (USART_BAUDRATE * 16UL))) - 1)

#define UBRR0L ((uint8_t)(F_CPU/(BAUD_RATE*16L)-1))
#define UBRR0H = ((F_CPU/(BAUD_RATE*16L)-1) >> 8)*/
#include <Arduino.h>

int main (void)
{
	init();

	Serial.begin(19200);
	while (true) {
			Serial.println("Hello world!");
	}
	/*uint8_t ch = 'a';

	initUART();

	while (1) {
		while (!uartTx(ch)) { }
		ch = ch == 'z' ? 'a' : (ch+1);
	}*/
}