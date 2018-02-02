// Access control: CalSTAR MR

#include <Arduino.h>

#define F_CPU 16000000
#define BAUD_RATE 19200

int main() {
  init(); // always include this when using Arduino.h!

  Serial.begin(BAUD_RATE);
  Serial.println("Serial interface initialized with a baud rate of %i", BAUD_RATE);
  while(true) {
    char* input = Serial.readString(); // get user input
    Serial.println("Message received! You said \"%s\"", input); // send user input back to verify data is good
  }
}
