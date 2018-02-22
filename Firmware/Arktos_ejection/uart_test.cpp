// Access control: CalSTAR MR

#include <Arduino.h>

#define BAUD_RATE 19200

int main() {
  init(); // always include this when using Arduino.h!

  Serial.begin(BAUD_RATE);
  Serial.print("Serial interface initialized with a baud rate of ");
  Serial.println(BAUD_RATE);
  while(true) {
    String input = Serial.readString(); // get user input
    Serial.print("Message received! You said ");
    Serial.println(input); // send user input back to verify data is good
  }
}
