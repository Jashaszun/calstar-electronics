#define ARDUINO 100

// #include <Arduino.h>
#include "NewPing.h"

#define LEFT_SONAR_PING 2
#define RIGHT_SONAR_PING 4

#define SERIAL_BAUD 19200

NewPing leftSonar(LEFT_SONAR_PING, LEFT_SONAR_PING, 180);
NewPing rightSonar(RIGHT_SONAR_PING, RIGHT_SONAR_PING, 180);

int main(){
	Serial.begin(SERIAL_BAUD);
	while(1){
		Serial.print("Left Ultrasonic Reading: ");
		Serial.println(leftSonar.ping_in());
		Serial.print("Right Ultrasonic Reading: ");
		Serial.println(rightSonar.ping_in());
	}
}
