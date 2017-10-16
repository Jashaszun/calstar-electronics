#include <stdlib.h>
#include <stdio.h>
#include <avr/io.h>

#define F_CPU (16000000UL)
#define USART_BAUDRATE 19200
#define UBRR_VALUE (((F_CPU / (USART_BAUDRATE * 16UL))) - 1)
#include "i2c.h"
#include "ITG-3200.h"
#include "uart.h"


#define FOSC (16000000UL)
#define ITG3200_R 0xD3	// ADD pin is pulled high
#define ITG3200_W 0xD2	// So address is 0x69

#define sbi(var, mask)   ((var) |= (uint8_t)(1 << mask))
#define cbi(var, mask)   ((var) &= (uint8_t)~(1 << mask))

char ITG3200Read(unsigned char address);
void ITG3200Write(unsigned char address, unsigned char data);
void getITG3200(int average, int *readings);


int main() {

	i2cInit();
	initUART();
    delay_ms(100);

    ITG3200Write(PWR_M, 0x80);	// Reset to defaults
	ITG3200Write(SMPL, 0x00);	// SMLPRT_DIV = 0
	ITG3200Write(DLPF, 0x19);	// DLPF_CFG = 0, FS_SEL = 3
	ITG3200Write(INT_C, 0x05);	// Generate interrupt when device is ready or raw data ready
	ITG3200Write(PWR_M, 0x00);
	
    delay_ms(1000);
    
	int xyz_values[3];

    while (1) {
        getITG3200(10, xyz_values);
        // these are from calibration
        xyz_values[0] += 36;
        xyz_values[1] -= 17;
        xyz_values[2] += 14;

        char strBuffer[100];
        sprintf(strBuffer, "x: %+d; y: %+d; z: %+d\r\n", xyz_values[0], xyz_values[1], xyz_values[2]);
		
		while (!txStr(strBuffer, -1)) {}
        delay_ms(10);
    }
}

char ITG3200Read(unsigned char address) {
	char data;
	
	cbi(TWCR, TWEN);	// Disable TWI
	sbi(TWCR, TWEN);	// Enable TWI
	
	i2cSendStart();
	i2cWaitForComplete();
	
	i2cSendByte(ITG3200_W);	// write 0xD2
	i2cWaitForComplete();
	
	i2cSendByte(address);	// write register address
	i2cWaitForComplete();
	
	i2cSendStart();
	
	i2cSendByte(ITG3200_R);	// write 0xD3
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

void ITG3200Write(unsigned char address, unsigned char data) {
	i2cSendStart();
	i2cWaitForComplete();
	
	i2cSendByte(ITG3200_W);	// write 0xB4
	i2cWaitForComplete();
	
	i2cSendByte(address);	// write register address
	i2cWaitForComplete();
	
	i2cSendByte(data);
	i2cWaitForComplete();
	
	i2cSendStop();
}

void getITG3200(int average, int *readings) {
	char temp;
	signed int gx[average], gy[average], gz[average];
	signed int gyrox = 0;
	signed int gyroy = 0;
	signed int gyroz = 0;
	unsigned int i;
	
	for (i = 0; i<average; i++)
	{
		while (!(ITG3200Read(INT_S) & 0x01))
		;
		temp = 0;
		temp = ITG3200Read(GY_H);
		gy[i] = temp << 8;
		gy[i] |= ITG3200Read(GY_L);

		//while (!(ITG3200Read(INT_S) & 0x01))
		//;
		temp = 0;
		temp = ITG3200Read(GZ_H);
		gz[i] = temp << 8;
		gz[i] |= ITG3200Read(GZ_L);
		
		//while (!(ITG3200Read(INT_S) & 0x01))
		//;
		temp = 0;
		temp = ITG3200Read(GX_H);
		gx[i] = temp << 8;
		gx[i] |= ITG3200Read(GX_L);

		
		gyrox += gx[i];
		gyroy += gy[i];
		gyroz += gz[i];
	}
	
	gyrox = gyrox/average;
	gyroy = gyroy/average;
	gyroz = gyroz/average;
    
    readings[0] = gyrox;
    readings[1] = gyroy;
    readings[2] = gyroz;

}