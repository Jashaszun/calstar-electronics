#include <Arduino.h>
#include <Servo.h>

#define ESC1_DIR 2
#define ESC1_PWM 3

#define ESC2_DIR 1
#define ESC2_PWM 4

void defaultTest();

//Servo ESC1;

int main(void) {
	init();
	pinMode(ESC1_PWM, OUTPUT);
	pinMode(ESC1_DIR, OUTPUT);

    //Serial.begin(19200);
	//ESC1.attach(ESC1_PWM);

	while (true) {
        // if (Serial.available() > 0){
        //     int value = Serial.read();
        //     if (value >= 0 || value < 256){
        //         analogWrite(ESC1_PWM, value);
        //     }
        // }
		digitalWrite(ESC1_PWM, 1);
		//ESC1.write(180);
        //defaultTest();
	}

  	return 0;
}

void defaultTest(){
    //Serial.print("Writing ESC1 to high speed");
    analogWrite(ESC1_PWM, 255);

    delay(1000);

    //Serial.print("Writing ESC1 to half(ish) speed");
    analogWrite(ESC1_PWM, 125);

    delay(1000);

    //Serial.print("Reversing ESC1 direction");
    digitalWrite(ESC1_DIR, 0);

    delay(1000);

    //Serial.print("Reversing ESC1 direction again");
    digitalWrite(ESC1_DIR, 1);

    delay(1000);

    //Serial.print("Writing ESC1 to off speed");
    analogWrite(ESC1_PWM, 0);

    delay(1000);
}
