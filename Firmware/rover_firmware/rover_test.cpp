#include <Arduino.h>

#define ESC1_DIR 2
#define ESC1_PWM 3

#define ESC2_DIR 1
#define ESC2_PWM 4

#define L_SKID_SERVO 14
#define R_SKID_SERVO 13
#define SOLAR_SERVO 15

int main(void) {
	init();

	pinMode(L_SKID_SERVO, OUTPUT);
	pinMode(R_SKID_SERVO, OUTPUT);
	pinMode(SOLAR_SERVO, OUTPUT);
	pinMode(ESC1_PWM, OUTPUT);
	pinMode(ESC1_DIR, OUTPUT);
	pinMode(ESC2_PWM, OUTPUT);
	pinMode(ESC2_DIR, OUTPUT);

	// Serial.begin(19200);
	delay(1000);
	analogWrite(R_SKID_SERVO, 23);
	delay(100);
	analogWrite(L_SKID_SERVO, 22);
	delay(200);
	while(1) {
		// analogWrite(14, 0);
		// analogWrite(13, 0);
		// leftSkidServo.write(0);
		// delay(1000);
		digitalWrite(ESC1_DIR, 0);
		digitalWrite(ESC2_DIR, 0);
		// leftSkidServo.write(60);
		delay(200);
		analogWrite(ESC1_PWM, 100);
		analogWrite(ESC2_PWM, 100);
		// leftSkidServo.write(120);
		delay(200);
	}
}