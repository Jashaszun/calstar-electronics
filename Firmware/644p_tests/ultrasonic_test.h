#include <avr/io.h>
#include <util/delay.h>
#include <stdlib.h>

#define F_CPU 16000000
#define USART_BAUDRATE 19200
#define UBRR_VALUE (((F_CPU / (USART_BAUDRATE * 16UL))) - 1)

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

void setup();
int main();
unsigned int test_us1(unsigned short units);
unsigned int test_us2(unsigned short units);
void initUART(void);
uint8_t tx(uint8_t data);
uint8_t txStr(char *str, uint8_t buffer_size);
