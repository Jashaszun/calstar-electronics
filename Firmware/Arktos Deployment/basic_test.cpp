#include <Arduino.h>

int main(void) {
	init();

	pinMode(A0, INPUT); // ejection signal

	pinMode(5, OUTPUT); // red
	pinMode(6, OUTPUT); // green
	pinMode(7, OUTPUT); // blue

	for (int i = 0; i < 3; i++) {
		digitalWrite(5, HIGH);
		delay(500);
		digitalWrite(5, LOW);
		delay(500);
		digitalWrite(6, HIGH);
		delay(500);
		digitalWrite(6, LOW);
		delay(500);
		digitalWrite(7, HIGH);
		delay(500);
		digitalWrite(7, LOW);
		delay(500);
	}

	Serial.begin(19200);

	while (true) {
		Serial.print("Reading: ");
		bool input = digitalRead(A0);
		if (input) {
			Serial.println("high");
		} else {
			Serial.println("low");
		}
		digitalWrite(5, digitalRead(A0) ? HIGH : LOW);
	}

  	return 0;
}