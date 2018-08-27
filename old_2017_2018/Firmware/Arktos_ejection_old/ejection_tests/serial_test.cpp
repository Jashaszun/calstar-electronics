/*
Makefile:

PSRC = serial_test.cpp

# No extra arduino libraries
ARDLIBS =

*/

#include <Arduino.h>

int main() {
    init();
    Serial.begin(19200);
    Serial.println("Beginning serial test program.");
    Serial.println("Should recieve messages in format \"Loop: x\" where x is 0..");
    for (int i = 0; ; ++i) {
        Serial.println("Loop: " + i);
    }
}