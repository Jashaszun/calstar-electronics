#include <Arduino.h>

#define STEP 2
#define DIR 3

int main() {
    init();

    Serial.begin(19200);
    Serial.println("Beginning stepper motor test");

    pinMode(STEP, OUTPUT);
    pinMode(DIR, OUTPUT);

    digitalWrite(STEP, LOW);
    digitalWrite(DIR, LOW);

    while (true) {
        String input = Serial.readString();
        int step = atoi(input.c_str());

        Serial.print("Read value: ");
        Serial.println(step);

        if (step < 0) {
            digitalWrite(DIR, LOW);
            step = -step;
        } else {
            digitalWrite(DIR, HIGH);
        }

        // because low amps, cant go much more than 500 steps / second
        for (; step > 0; --step) {
            digitalWrite(STEP, HIGH);
            delayMicroseconds(1000);
            digitalWrite(STEP, LOW);
            delayMicroseconds(1000);
        }
    }
}