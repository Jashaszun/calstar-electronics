/*
Makefile:

PSRC = altimeter_test.cpp

ARDLIBS = Wire MPL3115A2

*/

#include <Arduino.h>
#include <SparkFunMPL3115A2.h>

int main() {
    init();
    Serial.begin(19200);
    Serial.println("Beginning altimeter test program.");
    Serial.println("Should recieve messages (via uart) in format \"Alt: num\".");
    Serial.println("Readings will be in feet.");

    Wire.begin();

    MPL3115A2 altimeter;
    altimeter.begin();
    altimeter.setModeAltimeter();
    altimeter.setOversampleRate(7);
    altimeter.enableEventFlags();

    while (true) {
        Serial.print("Alt: ");
        Serial.println(altimeter.readAltitudeFt());
    }

}