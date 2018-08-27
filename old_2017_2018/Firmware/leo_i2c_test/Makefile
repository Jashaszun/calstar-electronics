
CC = avr-gcc
OBJCOPY = avr-objcopy
AVRDUDE = avrdude
AVRDUDEFLAGS =  -c usbtiny -p atmega328p -U flash:w:
COMPFLAGS = -mmcu=atmega328p -I/usr/local/Cellar/avr-gcc/7.2.0/avr/include/ -Iext/i2cmaster/

TARGET = i2c

all: i2c.bin

clean:
	rm -f *.o *.bin *.hex

upload: i2c.hex
	$(AVRDUDE) $(AVRDUDEFLAGS)i2c.hex

i2c: i2c.o
	$(CC) i2c.o ext/i2cmaster/i2cmaster.o -o i2c

%.o: %.c
	$(CC) $(COMPFLAGS) -c $< -o $(@F)

%.bin: %.o ext/i2cmaster/i2cmaster.o
	$(CC) $(COMPFLAGS) ext/i2cmaster/i2cmaster.o $< -o $(@F)

%.hex: %.bin
	$(OBJCOPY) -j .text -j .data -O ihex $< $(@F)
