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
  initUART();
  PORT_US1Trig &= ~(MASK_US1Trig); // set trig pin low
  PORT_US2Trig &= ~(MASK_US2Trig); // set trig pin low
  while(1) {
    _delay_ms(10);
    string dist = itoa(test_us1(CENTIMETER)); // get data from US1 and convert to string
    // string dist = itoa(test_us2(CENTIMETER)); // get data from US2 and convert to string
    while(!txStr(dist1, -1)); // send on UART
  }
}

unsigned int test_us1(unsigned short units) {
  unsigned int microseconds = 0;
  PORT_US1Trig |= MASK_US1Trig; // set trig pin high
  _delay_us(10);
  PORT_US1Trig &= ~(MASK_US1Trig); // set trig pin low
  while (!(PIN_US1Echo & MASK_US1Echo)); // wait for echo to be pulled high
  while (PIN_US1Echo & MASK_US1Echo) { // count how long echo is pulled high
    microseconds += 1;
    _delay_us(1);
  }
  if (units == CENTIMETER) {
    return microseconds / 58;
  } else if (units == INCH) {
    return microseconds / 148;
  } else {
    return 0; // ERROR
  }
}

unsigned int test_us2(unsigned short units) {
  unsigned int microseconds = 0;
  PORT_US2Trig |= MASK_US2Trig; // set trig pin high
  _delay_us(10);
  PORT_US2Trig &= ~(MASK_US2Trig); // set trig pin low
  while (!(PIN_US2Echo & MASK_US2Echo)); // wait for echo to be pulled high
  while (PIN_US2Echo & MASK_US2Echo) { // count how long echo is pulled high
    microseconds += 1;
    _delay_us(1);
  }
  if (units == CENTIMETER) {
    return microseconds / 58;
  } else if (units == INCH) {
    return microseconds / 148;
  } else {
    return 0; // ERROR
  }
}

void initUART(void) { // copied from gyro_uart_test
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

uint8_t tx(uint8_t data) { // copied from gyro_uart_test
	// wait for byte to be transmitted
	if (!(UCSR0A & (1 << UDRE0))) {
		return 0;
	}
	// transmit data
	UDR0 = data;
	return 1;
}

uint8_t txStr(char *str, uint8_t buffer_size) { // copied from gyro_uart_test
	static uint8_t index = 0;

	if (str == 0) {
		index = 0;
		return 1;
	}

	if (index == buffer_size || str[index] == '\0') {
		index = 0;
		return 1;
	}
	if (tx(str[index])) {
		index++;
	}
	return 0;
}
