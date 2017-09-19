#include <avr/io.h>

#define USART_BAUDRATE 9600
#define F_CPU 16000000UL
#define UBRR_VALUE (((F_CPU / (USART_BAUDRATE * 16UL))) - 1)

/*#include <util/delay.h>*/

#define pin (PC3)

void initUART(void)
{
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
void sendUARTByte(uint8_t data)
{
	// Wait for byte to be transmitted
	while (!(UCSR0A & (1<<UDRE0))) { }
	// Transmit data
	UDR0 = data;
}
uint8_t sendUARTByte2(uint8_t data) {
	// wait for byte to be transmitted
	if (!(UCSR0A & (1 << UDRE0))) {
		return 0;
	}
	// transmit data
	UDR0 = data;
	return 1;
}
uint8_t receiveUARTByte()
{
	// Wait for byte to be received
	while (!(UCSR0A & (1 << RXC0))) { }
	// Receive data
	return UDR0;
}
uint8_t receiveUARTByte2(uint8_t *input) {
	// wait for byte to be received
	if (!(UCSR0A & (1 << RXC0))) {
		return 0;
	}
	// Receive data
	*input = UDR0;
	return 1;
}
int main (void)
{
	uint8_t input;

	/*DDRC = _BV(pin);
	PORTC = 0;*/

	initUART();
    /*while(1)
    {
		input = receiveUARTByte();
    	sendUARTByte(input);
        PORTC |= _BV(pin);

		input = receiveUARTByte();
    	sendUARTByte(input);
		PORTC &= ~_BV(pin);
    }*/

	while (1) {
		while (!receiveUARTByte2(&input)) ;
		while (!sendUARTByte2(input)) ;
	}
}