#include <Arduino.h>
#include <Servo.h>

Servo test1;
Servo test2;

int main(void) {
	init();

	test1.attach(3);
	test2.attach(5);

	Serial.begin(19200);
	char input[4];
	int inputIndex = 0;
	while (true) {
		if (Serial.available()) {
			char ch = Serial.read();
			if (ch == '\n') {
				bool in0Valid = input[0] == 'a' || input[0] == 'b';
				bool in1Valid = input[1] >= '0' && input[1] <= '9';
				bool in2Valid = input[2] >= '0' && input[2] <= '9';
				bool in3Valid = input[3] >= '0' && input[3] <= '9';

				int value = -1;
				if (inputIndex == 2 && in0Valid && in1Valid) {
					value = input[1]-'0';
				} else if (inputIndex == 3 && in0Valid && in1Valid && in2Valid) {
					value = (input[1]-'0')*10 + (input[2]-'0');
				} else if (inputIndex >= 4 && in0Valid && in1Valid && in2Valid && in3Valid) {
					value = (input[1]-'0') * 100 + (input[2]-'0') * 10 + (input[3]-'0');
				}

				if (value != -1) {
					Serial.print("Servo ");
					if (input[0] == 'a') {
						Serial.print("A -> ");
					} else {
						Serial.print("B -> ");
					}
					Serial.println(value);
					if (input[0] == 'a') {
						test1.write(value);
					} else {
						test2.write(value);
					}
				} else {
					Serial.println("Invalid.");
				}
				inputIndex = 0;
			} else if (inputIndex < 4) {
				input[inputIndex++] = ch;
			} else {
				inputIndex++;
			}
		}
	}

	return 0;
}