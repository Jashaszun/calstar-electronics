#include "DIO.h"
#include "Timer.h"

class Buzzer {
public:
  /* Specifies the port and pin of the buzzer and sets it to output mode. Should be off by default.
  */
  Buzzer(uint8_t port, uint8_t pin);
  /* In theory, the buzzer should work by responding to changes in voltage. This function
     flips the voltage on the buzzer pin whenever it is called, thus causing the buzzer to
     buzz as long as this function is being called.
  */
  void buzz();
private:
  long _lastFlip;
  uint8_t _lastState;
  uint8_t _port;
  uint8_t _pin;
  short period; // in millis
}