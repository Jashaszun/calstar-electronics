// This code borrows from http://www.embedds.com/programming-avr-i2c-interface/
// Access control: 

#define F_CPU 16000000
// 115200 baud
#define BITRATE 62 // ( clock / baud - 16) / 2
#define I2C_ADDR = 0x6B;

int main() {
	TWSR = 0;
	TWBR = BITRATE;
}

void i2c_init() {
	TWSR = 0;
	TWBR = BITRATE;
}

void i2c_start() {
	TWCR = (1 << TWEN) | (1 << TWSTA) | (1 << TWINT);
	while !(TWCR & (1 << TWINT));
}

void i2c_stop() {
	TWCR = (1 << TWEN) | (1 << TWSTO) | (1 << TWINT);
}
