#include <Arduino.h>
#include <NewPing.h>

#define LEFT_SONAR_PING 2
#define RIGHT_SONAR_PING 4

int main(void) {
	init();

	NewPing leftSonar(LEFT_SONAR_PING, LEFT_SONAR_PING, 180);
    NewPing rightSonar(RIGHT_SONAR_PING, RIGHT_SONAR_PING, 180);

	Serial.begin(19200);
	while (1) { 
        float leftPing = leftSonar.ping_in();
        float rightPing = rightSonar.ping_in();
        Serial.print("Left: ");
        Serial.print(leftPing);
        Serial.print(", right: ");
        Serial.println(rightPing);
        delay(100);
	}
}
