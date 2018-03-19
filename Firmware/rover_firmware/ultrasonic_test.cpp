#include <Arduino.h>
#include <NewPing.h>

#define LEFT_SONAR_TRIG 20
#define LEFT_SONAR_ECHO 21
#define RIGHT_SONAR_TRIG 19
#define RIGHT_SONAR_ECHO 18

int main(void) {
	init();

    NewPing leftSonar(LEFT_SONAR_TRIG, LEFT_SONAR_ECHO, 180);
    NewPing rightSonar(RIGHT_SONAR_TRIG, RIGHT_SONAR_ECHO, 180);

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
