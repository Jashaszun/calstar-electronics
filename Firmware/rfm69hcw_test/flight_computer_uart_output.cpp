#include <Arduino.h>

int main(void) {
	init();

	Serial.begin(19200);
	while (true) {
		for (char ch = 'a'; ch <= 'z'; ch++) {
			Serial.print(ch);
		}
		Serial.println();
	}
}