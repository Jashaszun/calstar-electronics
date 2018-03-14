#include <Arduino.h>
#include <Servo.h>

#define L_SKID_SERVO 5
#define R_SKID_SERVO 6
#define SOLAR_SERVO 3

Servo leftSkidServo;
Servo rightSkidServo;
Servo solarServo;

void initPins(void) {
	leftSkidServo.attach(L_SKID_SERVO);
	leftSkidServo.write(90);

	rightSkidServo.attach(R_SKID_SERVO);
	rightSkidServo.write(90);

	solarServo.attach(SOLAR_SERVO);
	solarServo.write(90);
}

void testTenTimes(Servo servo) {
	for (int i = 0; i < 10; i++) {
		randomMove(servo);
	}
}

void randomMove(Servo servo) {
	int randNum = rand() % (180 + 1 - 0) + 0
	Serial.println(randNum);
	servo.write(angle);
	delay(2000);
}

int main(void) {
	init();
	initPins();

	Serial.begin(19200);

	//test left servo 
	testTenTimes(leftSkidServo);

	//test right servo
	testTenTimes(rightSkidServo);

	//test solar servo
	testTenTimes(solarServo);
}