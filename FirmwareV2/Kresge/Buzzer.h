#include "DIO.h"
#include "Timer.h"

class Buzzer {
public:
  /* Specifies the port and pin of the buzzer and sets it to output mode. Should be off by default. */
  Buzzer(uint8_t port, uint8_t pin);
  /* The rest of the functions will set the state of the buzzer so that it will buzz if necessary
   if updateBuzzer is called within the main control loop.
   Starts a blocking buzz at a given duration at a given frequency.
  */
  void blockingBuzz(long duration_us, short frequency);
  /* Starts a blocking buzz at a default frequency of 500 Hz
  */
  void blockingBuzz(long duration_us);
  /* Starts a non-blocking buzz with a given duration and frequency.
   If this (or any other startBuzz) is called while a buzz is already happening, the new buzz will take precedent.
  */
  void startBuzz(long duration_us, short frequency);
  /* Starts a non-blocking buzz at a default frequency of 500 Hz
  */
  void startBuzz(long duration_us);
  void stopBuzz();
  /* The buzzer buzzes at the frequency at which the voltage changes. This function should be called
   on every frame of a control loop, and will flip the voltage if the buzzer is active.
   Returns whether or not the buzzer voltage was flipped.
  */
  bool updateBuzzer();
  /* Returns whether the buzzer is active. */
  bool isBuzzing();
private:
  long micros();
  uint8_t _pin;
  uint8_t _port;
  long _lastFlip;
  long _lastDuration;
  long _lastStart;
  bool _lastState;
  bool _active;
  short _period; // in us
  const float PI = 3.14159265358979;
  const short DEFAULT_FREQUENCY = 500; // Hz
  void _setValuesOnBuzzStart(long duration_us, short frequency); // should be called on all startBuzz calls
  void _flipVoltage();
};