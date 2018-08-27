#include <Arduino.h>

#define ESC1_DIR 2
#define ESC1_PWM 3

#define ESC2_DIR 1
#define ESC2_PWM 4

int main(void) {
	init();
	Serial.begin(19200);

	pinMode(ESC1_DIR, OUTPUT);
	pinMode(ESC1_PWM, OUTPUT);
	pinMode(ESC2_DIR, OUTPUT);
	pinMode(ESC2_PWM, OUTPUT);
	
	digitalWrite(ESC1_DIR, 0);
	digitalWrite(ESC1_PWM, 0);
	digitalWrite(ESC2_DIR, 1);
	digitalWrite(ESC2_PWM, 0);

	while(1) {
		
		//run at 20% duty cycle
		analogWrite(ESC1_PWM, 50);
		analogWrite(ESC2_PWM, 50);
		delay(200);
		
		//run at 40% duty cycle
		analogWrite(ESC1_PWM, 100);
		analogWrite(ESC2_PWM, 100);
		delay(200);
		
		//run at 80% duty cycle
		analogWrite(ESC1_PWM, 200);
		analogWrite(ESC2_PWM, 200);
		delay(200);
		
		//alternate both motor directions
		digitalWrite(ESC1_DIR, !digitalRead(ESC1_DIR));
		digitalWrite(ESC2_DIR, !digitalRead(ESC2_DIR));
		delay(300);
	}
}

// void defaultTest(){
//     //Serial.print("Writing ESC1 to high speed");
//     analogWrite(ESC1_PWM, 255);

//     delay(1000);

//     //Serial.print("Writing ESC1 to half(ish) speed");
//     analogWrite(ESC1_PWM, 125);

//     delay(1000);

//     //Serial.print("Reversing ESC1 direction");
//     digitalWrite(ESC1_DIR, 0);

//     delay(1000);

//     //Serial.print("Reversing ESC1 direction again");
//     digitalWrite(ESC1_DIR, 1);

//     delay(1000);

//     //Serial.print("Writing ESC1 to off speed");
//     analogWrite(ESC1_PWM, 0);

//     delay(1000);
// }
