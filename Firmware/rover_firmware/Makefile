# the name of your main file, with no extension
TARGET = servo_test
# the C source files
SRC =
# the C++ source files
PSRC = servo_test.cpp
# extra Arduino libraries
ARDLIBS = Servo
ARDCORELIBS = WInterrupts WMath hooks wiring wiring_digital wiring_analog HardwareSerial HardwareSerial0 Print Stream WString abi
MCU = atmega644p
AVRDUDE_MCU = m644p
AVRDUDE_PORT = COM3 //change

ARDLIBDIR = $(ARDDIR)/mc_libraries
ARDCOREDIR = $(ARDDIR)/mc_core

include ../Makefile.base
