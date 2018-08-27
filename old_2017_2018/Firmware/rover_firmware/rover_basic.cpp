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

	// while(1){
	// 	delay(300);
	// 	analogWrite(SOLAR_SERVO, 8);
	// 	delay(300);
	// 	analogWrite(SOLAR_SERVO, 28);
	// }
	// Serial.begin(19200);
	
	//Reset servos
	delay(300);
	analogWrite(R_SKID_SERVO, 8);
	delay(200);
	analogWrite(L_SKID_SERVO, 8);
	analogWrite(SOLAR_SERVO, 8);
	delay(500);

	//Deploy Servos
	analogWrite(L_SKID_SERVO, 23);
	delay(200);
	analogWrite(R_SKID_SERVO, 22);
	delay(200);

	//Initiate movement (9 seconds)
	digitalWrite(ESC1_DIR, 0);
	digitalWrite(ESC2_DIR, 1);
	analogWrite(ESC1_PWM, 50);
	analogWrite(ESC2_PWM, 50);
	delay(100);
	analogWrite(ESC1_PWM, 100);
	analogWrite(ESC2_PWM, 100);
	delay(1600);

	//stop movement
	analogWrite(ESC1_PWM, 0);
	analogWrite(ESC2_PWM, 0);
	delay(200);

	// Deploy solar
	analogWrite(SOLAR_SERVO, 22);
}