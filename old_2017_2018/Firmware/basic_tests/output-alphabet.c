#include <avr/io.h>

#define USART_BAUDRATE 19200
#define F_CPU 16000000UL
#define UBRR_VALUE (((F_CPU / (USART_BAUDRATE * 16UL))) - 1)

void initUART(void) {
	// Set baud rate
	UBRR0H = (uint8_t)(UBRR_VALUE >> 8);
	UBRR0L = (uint8_t)UBRR_VALUE;
	// Set frame format to 8 data bits, no parity, 1 stop bit
	UCSR0C |= (1 << UCSZ01) | (1 << UCSZ00);
	//enable transmission and reception
	UCSR0B |= (1 << RXEN0) | (1 << TXEN0);

	DDRD &= ~_BV(PD0);
	PORTD |= _BV(PD0);
}
uint8_t uartRx(uint8_t *input) {
	// wait for byte to be received
	if (!(UCSR0A & (1 << RXC0))) {
		return 0;
	}
	// Receive data
	*input = UDR0;
	return 1;
}
uint8_t uartTx(uint8_t data) {
	// wait for byte to be transmitted
	if (!(UCSR0A & (1 << UDRE0))) {
		return 0;
	}
	// transmit data
	UDR0 = data;
	return 1;
}
uint8_t uartTxStr(char *str, uint8_t buffer_size) {
	static uint8_t index = 0;

	if (str == 0) {
		index = 0;
		return 1;
	}

	if (*str == '\0' || index == buffer_size) {
		index = 0;
		return 1;
	}
	if (uartTx(str[index])) {
		index++;
	}
	return 0;
}
void byteToStr(uint8_t b, char *s) {
	uint8_t ones = b % 10;
	b -= ones;
	uint8_t tens = (b / 10) % 10;
	b -= tens * 10;
	uint8_t hundreds = b ? 1 : 0;

	s[0] = '0' + hundreds;
	s[1] = '0' + tens;
	s[2] = '0' + ones;
}
int main (void)
{
	uint8_t ch = 'a';

	initUART();

	while (1) {
		while (!uartTx(ch)) { }
		ch = ch == 'z' ? 'a' : (ch+1);
	}
}