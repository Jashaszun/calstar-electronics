#include <Arduino.h>
#include <Servo.h>

#define L_SKID_SERVO 5
#define R_SKID_SERVO 6
#define SOLAR_SERVO 3

Servo leftSkidServo;
Servo rightSkidServo;
Servo solarServo;

void randomMove(Servo servo);

int pos = 0;

void initPins(void) {
	leftSkidServo.attach(L_SKID_SERVO);
	leftSkidServo.write(90);

	rightSkidServo.attach(R_SKID_SERVO);
	rightSkidServo.write(90);

	solarServo.attach(SOLAR_SERVO);
	solarServo.write(90);
}

//test random angles 10 times
void testTenTimes(Servo servo) {
	for (int i = 0; i < 10; i++) {
		randomMove(servo);
	}
}

//randomly sets an angle on servo and prints out on serial
void randomMove(Servo servo) {
	int randNum = rand() % (180 + 1 - 0) + 0;
	Serial.println(randNum);
	servo.write(randNum);
	delay(2000);
}


void loop(Servo myservo) {
  for (pos = 0; pos <= 180; pos += 1) { // goes from 0 degrees to 180 degrees
    // in steps of 1 degree
    myservo.write(pos);              // tell servo to go to position in variable 'pos'
    delay(15);                       // waits 15ms for the servo to reach the position
  }
  for (pos = 180; pos >= 0; pos -= 1) { // goes from 180 degrees to 0 degrees
    myservo.write(pos);              // tell servo to go to position in variable 'pos'
    delay(15);                       // waits 15ms for the servo to reach the position
  }
}

int main(void) {
	init();
	initPins();

	Serial.begin(19200);

	//sweep servo
	loop(leftSkidServo);
	loop(rightSkidServo);
	loop(solarServo);

	//test servos randomly 
	testTenTimes(leftSkidServo);
	testTenTimes(rightSkidServo);
	testTenTimes(solarServo);
}