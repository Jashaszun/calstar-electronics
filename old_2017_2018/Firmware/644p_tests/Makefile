all:
	avr-gcc ultrasonic_test.cpp -mmcu=atmega644p -DF_CPU="16000000"
	avr-objcopy -O ihex -R .eeprom a.out a.hex

program: all
	avrdude -p m644p -c avrispv2 -U flash:w:a.hex -P COM12
