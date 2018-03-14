#include <Arduino.h>
#include <Servo.h>

#define ESC1_DIR 42
#define ESC1_PWM 43

#define ESC2_DIR 41
#define ESC2_PWM 44

void defaultTest();

int main(void) {
	init();

    Serial.begin(19200);

	while (true) {
        // if (Serial.available() > 0){
        //     int value = Serial.read();
        //     if (value >= 0 || value < 256){
        //         analogWrite(ESC1_PWM, value);
        //     }
        // }

        defaultTest();
	}

  	return 0;
}

void defaultTest(){
    Serial.print("Writing ESC1 to high speed");
    analogWrite(ESC1_PWM, 255);

    delay(1000);

    Serial.print("Writing ESC1 to half(ish) speed")
    analogWrite(ESC1_PWM, 125);

    delay(1000)

    Serial.print("Reversing ESC1 direction")
    digitalWrite(ESC1_DIR, 0)

    delay(1000);

    Serial.print("Reversing ESC1 direction again")
    digitalWrite(ESC1_DIR, 1)

    delay(1000);

    Serial.print("Writing ESC1 to off speed")
    analogWrite(ESC1_PWM, 0);

    delay(1000)
}
