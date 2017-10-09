#include <stdlib.h>
#include <stdio.h>

#define USART_BAUDRATE 19200
#define F_CPU (16000000UL)
#define UBRR_VALUE (((F_CPU / (USART_BAUDRATE * 16UL))) - 1)

#include <avr/io.h>
#include "i2c.h"
#include "uart.h"
#include "LIS331.h"

#define FOSC (16000000UL)

#define LIS331_SAD 0b00110000
#define LIS331_R (LIS331_SAD | 0x01)
#define LIS331_W (LIS331_SAD | 0x00)

#define sbi(var, mask)   ((var) |= (uint8_t)(1 << mask))
#define cbi(var, mask)   ((var) &= (uint8_t)~(1 << mask))

char LIS331Read(unsigned char address);
void LIS331Write(unsigned char address, unsigned char data);
void getLIS331(int average, int *readings);

int main() {

	i2cInit();
	initUART();
    delay_ms(100);

    LIS331Write(CTRL_REG1, 0x27);
    LIS331Write(CTRL_REG4, 0x82);
	
    delay_ms(1000);
    
	int xyz_values[3];
    while (1) {
        getLIS331(10, xyz_values);

		char str_buf[100];
		sprintf(str_buf, "x: %+d; y: %+d; z: %+d\r\n", xyz_values[0], xyz_values[1], xyz_values[2]);
		while(!txStr(str_buf, -1)) {}
    }
}

char LIS331Read(unsigned char address) {
	char data;
	
	cbi(TWCR, TWEN);	// Disable TWI
	sbi(TWCR, TWEN);	// Enable TWI
	
	i2cSendStart();
	i2cWaitForComplete();
	
	i2cSendByte(LIS331_W);
	i2cWaitForComplete();
	
	i2cSendByte(address);	// write register address
	i2cWaitForComplete();
	
	i2cSendStart();
	
	i2cSendByte(LIS331_R);
	i2cWaitForComplete();
	i2cReceiveByte(FALSE);
	i2cWaitForComplete();
	
	data = i2cGetReceivedByte();	// Get MSB result
	i2cWaitForComplete();
	i2cSendStop();
	
	cbi(TWCR, TWEN);	// Disable TWI
	sbi(TWCR, TWEN);	// Enable TWI
	
	return data;
}

void LIS331Write(unsigned char address, unsigned char data) {
	i2cSendStart();
	i2cWaitForComplete();
	
	i2cSendByte(LIS331_W);
	i2cWaitForComplete();
	
	i2cSendByte(address);	// write register address
	i2cWaitForComplete();
	
	i2cSendByte(data);
	i2cWaitForComplete();
	
	i2cSendStop();
}

void getLIS331(int average, int *readings) {
	char temp;
	signed int ax[average], ay[average], az[average];
	signed int altx = 0;
	signed int alty = 0;
	signed int altz = 0;
	unsigned int i;
	
	for (i = 0; i<average; i++) {
		// Wait until there is some new reading
		while (!LIS331Read(STATUS_REG)) {}

		temp = 0;
		temp = LIS331Read(OUT_Y_H);
		ay[i] = temp << 8;
		ay[i] |= LIS331Read(OUT_Y_L);

		temp = 0;
		temp = LIS331Read(OUT_Z_H);
		az[i] = temp << 8;
		az[i] |= LIS331Read(OUT_Z_L);

		temp = 0;
		temp = LIS331Read(OUT_X_H);
		ax[i] = temp << 8;
		ax[i] |= LIS331Read(OUT_X_L);

		altx += ax[i];
		alty += ay[i];
		altz += az[i];
	}
	
	altx = altx/average;
	alty = alty/average;
	altz = altz/average;
    
    readings[0] = altx;
    readings[1] = alty;
    readings[2] = altz;    
}