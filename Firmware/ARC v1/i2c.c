// This code borrows from http://www.embedds.com/programming-avr-I2C-interface/
// Access control: 

#include <avr/io.h>

#define F_CPU 16000000
// 115200 baud
#define BITRATE 62 // ( clock / baud - 16) / (2 * prescaler)
#define I2C_ADDR 0x6B
#define I2C_READ 1
#define I2C_WRITE 0
#define X_ACCEL_MSB 0x28
#define X_ACCEL_LSB 0x29
#define Y_ACCEL_MSB 0x2A
#define Y_ACCEL_LSB 0x2B
#define Z_ACCEL_MSB 0x2C
#define Z_ACCEL_LSB 0x2D

void error() {
	// PORTB = 0xFF;
	// PORTD = 0xFF;
}

void I2C_init() {
	TWSR = 0; // prescaler = 1
	TWBR = BITRATE; // load into bitrate reg
}

void I2C_start() {
	TWCR = (1 << TWEN) | (1 << TWSTA) | (1 << TWINT); // transmit start condition
	while (!(TWCR & (1 << TWINT))); // wait for interrupt flag
	if (TWSR != 0x08) { // get status, should be 0x08
		error();
	}
}

void I2C_stop() {
	TWCR = (1 << TWEN) | (1 << TWSTO) | (1 << TWINT); // transmit stop condition
}

void I2C_write(char byte) {
	TWDR = byte;
	TWCR = (1 << TWINT) | (1 << TWEN); // clear interrupt flag to transmit
	while (!(TWCR & (1 << TWINT)));
	if ((TWSR == 0x38) || (TWSR == 0x48)) {
		error();
	} else { // ACK received
		TWCR |= (1 << TWINT); // clear ACK
	}
}

char I2C_read_nack() {
	TWCR = (1 << TWINT) | (1 << TWEN);
	while (!(TWCR & (1 << TWINT)));
	return TWDR;
}

/*short xAccel() {
	short x;
	I2C_start();
	I2C_write((I2C_ADDR << 1) + I2C_WRITE); // slave address, write mode
	I2C_write(X_ACCEL_MSB << 1) // register address
	I2C_start(); // repeated start
	I2C_write((I2C_ADDR << 1) + I2C_READ); // slave address, read mode
	x = (I2C_read_nack() << 8);
	I2C_stop();
	I2C_start();
	I2C_write((I2C_ADDR << 1) + I2C_WRITE); // slave address, write mode
	I2C_write(X_ACCEL_LSB << 1) // register address
	I2C_start(); // repeated start
	I2C_write((I2C_ADDR << 1) + I2C_READ); // slave address, read mode
	x |= I2C_read_nack();
	I2C_stop();
	return x;
}*/

short lsm9ds1_read(char addr1, char addr2) {
	short rv;
	I2C_start();
	I2C_write((I2C_ADDR << 1) + I2C_WRITE); // slave address, write mode
	I2C_write(addr1 << 1); // register address
	I2C_start(); // repeated start
	I2C_write((I2C_ADDR << 1) + I2C_READ); // slave address, read mode
	rv = (I2C_read_nack() << 8);
	I2C_stop();
	I2C_start();
	I2C_write((I2C_ADDR << 1) + I2C_WRITE); // slave address, write mode
	I2C_write(addr2 << 1); // register address
	I2C_start(); // repeated start
	I2C_write((I2C_ADDR << 1) + I2C_READ); // slave address, read mode
	rv |= I2C_read_nack();
	I2C_stop();
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
	I2C_init();
	int x = lsm9ds1_xAccel();
	PORTB = 0xFF;
	PORTD = 0xFF;
}

