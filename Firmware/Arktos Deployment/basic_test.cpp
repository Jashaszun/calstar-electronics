#include <Arduino.h>

int main(void) {
	init();

	pinMode(A0, INPUT); // ejection signal

	pinMode(5, OUTPUT); // red
	pinMode(6, OUTPUT); // green
	pinMode(7, OUTPUT); // blue

	digitalWrite(5, HIGH);
	delay(1000);
	digitalWrite(5, LOW);
	delay(1000);
	digitalWrite(6, HIGH);
	delay(1000);
	digitalWrite(6, LOW);
	delay(1000);
	digitalWrite(7, HIGH);
	delay(1000);
	digitalWrite(7, LOW);
	delay(1000);

	while (true) {
		digitalWrite(5, digitalRead(A0) ? HIGH : LOW);
	}

  	return 0;
}