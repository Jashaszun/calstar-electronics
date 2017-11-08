DEVICE := atmega328
INCLUDE := -IC:\Users\Neelay\WinAVR\avr\include
MAIN = main

all: main.c
	avr-gcc -g -mmcu=$(DEVICE) -c $(MAIN).c 
	avr-gcc -g -mmcu=$(DEVICE) -o $(MAIN).bin $(MAIN).o
	objcopy -j .text -j .data -O ihex $(MAIN).bin $(MAIN).hex

clean:
	rm $(MAIN).bin
	rm $(MAIN).o
	rm $(MAIN).hex

flash: all
	avrdude -c usbtiny -p m328 -U flash:w:$(MAIN).hex