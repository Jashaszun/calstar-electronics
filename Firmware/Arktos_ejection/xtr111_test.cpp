#include <Arduino.h>

#define CURRENTLOOP_OD 7 // output disable
#define CURRENTLOOP_EF 8 // error flag

int main() {
    init();
    Serial.begin(19200);

    Serial.println("Beginning XTR11 Test");

    pinMode(CURRENTLOOP_OD, OUTPUT);
    digitalWrite(CURRENTLOOP_OD, HIGH);

    pinMode(CURRENTLOOP_EF, INPUT_PULLUP);

    while (true) {
        if (Serial.available() == 2) {
            char first = Serial.read();
            char second = Serial.read();
            
            Serial.print("Recieved: ");Serial.print(first);Serial.println(second);

            if (first== 'o' && second == 'n') {
                digitalWrite(CURRENTLOOP_OD, LOW);
            } else if (first == 'o' && second == 'f') {
                digitalWrite(CURRENTLOOP_OD, HIGH);
            }
        }
    }
}