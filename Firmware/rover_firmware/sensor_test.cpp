#include <Arduino.h>
#include <NewPing.h>

#define LEFT_SONAR_PING 2
// #define RIGHT_SONAR_PING 4

int main(void){
	init();

	NewPing leftSonar(LEFT_SONAR_PING, LEFT_SONAR_PING);

	Serial.begin(115200);
	while(1){ 
		delay(50);
		unsigned int uS = leftSonar.ping();
		Serial.print("Left  Ultrasonic Reading: ");
		Serial.print(uS / US_ROUNDTRIP_CM);
		Serial.println("cm");
	}
	return 0;
}