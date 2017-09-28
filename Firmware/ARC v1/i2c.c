// This code borrows from http://www.embedds.com/programming-avr-i2c-interface/
// Access control: 

#define F_CPU 16000000
// 115200 baud
#define BITRATE 62 // ( clock / baud - 16) / (2 * prescaler)
#define I2C_ADDR 0x6B
#define I2C_READ 1
#define I2C_WRITE 0
#define X_ACCEL_MSB 0x28
#define X_ACCEL_LSB 0x29

int main() {
	TWSR = 0;
	TWBR = BITRATE;
}

void i2c_init() {
	TWSR = 0; // prescaler = 1
	TWBR = BITRATE; // load into bitrate reg
}

void i2c_start() {
	TWCR = (1 << TWEN) | (1 << TWSTA) | (1 << TWINT); // transmit start condition
	while !(TWCR & (1 << TWINT)); // wait for interrupt flag
	if (TWSR != 0x08) { // get status, should be 0x08
		error();
	}
}

void i2c_stop() {
	TWCR = (1 << TWEN) | (1 << TWSTO) | (1 << TWINT); // transmit stop condition
}

void i2c_write(char byte) {
	TWDR = byte;
	TWCR = (1 << TWINT) | (1 << TWEN); // clear interrupt flag to transmit
	while !(TWCR & (1 << TWINT));
	if ((TWSR == 0x38) || (TWSR == 0x48)) {
		error();
	} else { // ACK received
		TWCR |= (1 << TWINT); // clear ACK
	}
}

char i2c_read_nack() {
	TWCR = (1 << TWINT) | (1 << TWEN);
	while !(TWCR & (1 << TWINT));
	return TWDR;
}

short xAccel() {
	short x;
	i2c_start();
	i2c_write((I2C_ADDR << 1) + I2C_WRITE); // slave address, write mode
	i2c_write(X_ACCEL_MSB << 1) // register address
	i2c_start(); // repeated start
	i2c_write((I2C_ADDR << 1) + I2C_READ); // slave address, read mode
	x = (i2c_read_nack() << 8);
	i2c_stop();
	i2c_start();
	i2c_write((I2C_ADDR << 1) + I2C_WRITE); // slave address, write mode
	i2c_write(X_ACCEL_LSB << 1) // register address
	i2c_start(); // repeated start
	i2c_write((I2C_ADDR << 1) + I2C_READ); // slave address, read mode
	x |= i2c_read_nack();
	i2c_stop();
	return x;
}

void error() {
	// do something with the outputs here
}
