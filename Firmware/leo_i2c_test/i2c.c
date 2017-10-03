// This code borrows from http://www.embedds.com/programming-avr-i2c-interface/
// Access control:

#define F_CPU 16000000

#include <avr/io.h>
#include <util/delay.h>
#include <i2cmaster.h>
// 115200 baud
#define BITRATE 62 // ( clock / baud - 16) / (2 * prescaler)
#define I2C_ADDR 0xD6
#define X_ACCEL_MSB 0x28
#define X_ACCEL_LSB 0x29
#define Y_ACCEL_MSB 0x2A
#define Y_ACCEL_LSB 0x2B
#define Z_ACCEL_MSB 0x2C
#define Z_ACCEL_LSB 0x2D

void delay(int ms) {
    int repeats, remainder;
    repeats = ms / 10;
    remainder = ms % 10;
    int i;
    for (i = 0; i < repeats; i++) {
        _delay_ms(10);
    }
    _delay_ms(remainder);
}

void error() {
	PORTB = 0xFF;
	PORTD = 0xFF;
}


short lsm9ds1_read(char addr1, char addr2) {
	short rv;
	i2c_start_wait(I2C_ADDR + I2C_WRITE); // slave address, write mode
	i2c_write(addr1); // register address
	i2c_write(I2C_ADDR + I2C_READ); // slave address, read mode
	rv = (i2c_readNak() << 8);
	i2c_stop();
	i2c_start_wait(I2C_ADDR + I2C_WRITE); // slave address, write mode
	i2c_write(addr2); // register address
	i2c_stop();
	i2c_start_wait(I2C_ADDR + I2C_READ); // slave address, read mode
	i2c_write(0x40);
	rv |= i2c_readNak();
	i2c_stop();
	return rv;
}

short lsm9ds1_xAccel() {
	return lsm9ds1_read(X_ACCEL_MSB, X_ACCEL_LSB);
}

short lsm9ds1_yAccel() {
	return lsm9ds1_read(Y_ACCEL_MSB, Y_ACCEL_LSB);
}

short lsm9ds1_zAccel() {
	return lsm9ds1_read(Z_ACCEL_MSB, Z_ACCEL_LSB);
}

int main() {
	DDRD = 0xFF;
	DDRB = 0xFF;
	PORTD = 0x00;
	PORTB = 0x00;
	i2c_init();
	short x = lsm9ds1_xAccel();
	if (x == 0) {
		error();
	}
}
