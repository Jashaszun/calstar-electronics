all:
	avr-gcc gpio_test.cpp -mmcu=atmega644p -DF_CPU="16000000"
	avr-objcopy.exe -O ihex -R .eeprom a.out a.hex

program: all
	avrdude.exe -p m644p -c avrispv2 -U flash:w:a.hex -P COM12
