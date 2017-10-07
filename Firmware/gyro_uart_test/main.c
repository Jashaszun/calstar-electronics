#include <stdlib.h>
#include <stdio.h>
#include <avr/io.h>
// #include <util/delay.h>
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
void ITG3200ViewRegisters(void);
int checkInterrupt(void);
void getITG3200(int average, int *readings);

// void delay_ms(uint16_t x);

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
	char num_str[4];
	num_str[3] = '\0';
    while (1) {
        getITG3200(10, xyz_values);
        // these are from calibration
        xyz_values[0] += 36;
        xyz_values[1] -= 17;
        xyz_values[2] += 14;

        char strBuffer[100];
        sprintf(strBuffer, "x: %+d; y: %+d; z: %+d\r\n", xyz_values[0], xyz_values[1], xyz_values[2]);
		
		while (!txStr(strBuffer, -1)) {}

		/*int i;
		for (i = 0; i < 3; ++i) {
			byteToStr(((xyz_values[i] & ((uint32_t)0xFF << 24)) >> 24), num_str);
			while (!txStr(num_str, 3)) {}
			while (!txStr(", ", 3)) {}

			byteToStr(((xyz_values[i] & ((uint32_t)0xFF << 16)) >> 16), num_str);
			while (!txStr(num_str, 3)) {}
			while (!txStr(", ", 3)) {}

			byteToStr(((xyz_values[i] & ((uint32_t)0xFF << 8)) >> 8), num_str);
			while (!txStr(num_str, 3)) {}
			while (!txStr(", ", 3)) {}

			byteToStr((xyz_values[i] & 0xFF), num_str);
			while (!txStr(num_str, 3)) {}
			while (!txStr(", ", 3)) {}

			while (!txStr("; ", 2)) {}
		}
		while(!txStr("\r\n", 2))*/

        // put_char((unsigned char)((xyz_values[0] & (0xFF << 24)) >> 24));
        // put_char((unsigned char)((xyz_values[0] & (0xFF << 16)) >> 16));
        // put_char((unsigned char)((xyz_values[0] & (0xFF << 8)) >> 8));
        // put_char((unsigned char)((xyz_values[0] & 0xFF)));
        
        // put_char(4);
        // put_char((unsigned char)((xyz_values[1] & (0xFF << 24)) >> 24));
        // put_char((unsigned char)((xyz_values[1] & (0xFF << 16)) >> 16));
        // put_char((unsigned char)((xyz_values[1] & (0xFF << 8)) >> 8));
        // put_char((unsigned char)((xyz_values[1] & 0xFF)));

        // put_char(4);
        // put_char((unsigned char)((xyz_values[2] & (0xFF << 24)) >> 24));
        // put_char((unsigned char)((xyz_values[2] & (0xFF << 16)) >> 16));
        // put_char((unsigned char)((xyz_values[2] & (0xFF << 8)) >> 8));
        // put_char((unsigned char)((xyz_values[2] & 0xFF)));
        
        delay_ms(10);
    }
}

char ITG3200Read(unsigned char address)
{
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

void ITG3200Write(unsigned char address, unsigned char data)
{
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

void ITG3200ViewRegisters(void)
{
	// printf("\nWHO_AM_I (0x00): 0x%x\n", ITG3200Read(WHO));
	// delay_ms(1000);
	// printf("SMPLRT_DIV (0x15): 0x%x\n", ITG3200Read(SMPL));
	// printf("DLPF_FS (0x16): 0x%x\n", ITG3200Read(DLPF));
	// printf("INT_CFG (0x17): 0x%x\n", ITG3200Read(INT_C));
	// printf("INT_STATUS (0x1A): 0x%x\n", ITG3200Read(INT_S));
	// printf("TEMP_OUT_H (0x1B): 0x%x\n", ITG3200Read(TMP_H));
	// printf("TEMP_OUT_L (0x1C): 0x%x\n", ITG3200Read(TMP_L));
	// printf("GYRO_XOUT_H (0x1D): 0x%x\n", ITG3200Read(GX_H));
	// printf("GYRO_XOUT_L (0x1E): 0x%x\n", ITG3200Read(GX_L));
	// printf("GYRO_YOUT_H (0x1F): 0x%x\n", ITG3200Read(GY_H));
	// printf("GYRO_YOUT_L (0x20): 0x%x\n", ITG3200Read(GY_L));
	// printf("GYRO_ZOUT_H (0x21): 0x%x\n", ITG3200Read(GZ_H));
	// printf("GYRO_ZOUT_L (0x22): 0x%x\n", ITG3200Read(GZ_L));
	// printf("PWR_MGM (0x3E): 0x%x\n", ITG3200Read(PWR_M));
}

int checkInterrupt(void)
{
	if ((PIND & (1<<2)) == 0)
		return 0;
	else
		return 1;
}

void getITG3200(int average, int *readings)
{
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
    
	//printf("	%d	%d	%d\n", gyrox, gyroy, gyroz);
}