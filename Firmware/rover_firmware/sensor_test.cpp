#include <Arduino.h>
// #include <NewPing.h>

#define LEFT_SONAR_PING 2
// #define RIGHT_SONAR_PING 4

int main(void){
	init();
	pinMode(LEFT_SONAR_PING, OUTPUT);

	// NewPing leftSonar(LEFT_SONAR_PING, LEFT_SONAR_PING);

	Serial.begin(19200);
	while(1){ 
		analogWrite(LEFT_SONAR_PING, 0);
		delay(200);
		analogWrite(LEFT_SONAR_PING, 50);
		delay(200);
		analogWrite(LEFT_SONAR_PING, 100);
		delay(200);
		analogWrite(LEFT_SONAR_PING, 150);
		delay(200);
		analogWrite(LEFT_SONAR_PING, 200);
		delay(200);
		analogWrite(LEFT_SONAR_PING, 255);
		delay(200);
	}
}