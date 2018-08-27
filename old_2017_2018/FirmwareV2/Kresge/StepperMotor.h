#ifndef _KRESGE_STEPPERMOTOR_H
#define _KRESGE_STEPPERMOTOR_H
#include "common.h"

// For controlling bipolar 2-wire stepper motors
class StepperMotor {
private:
    float accel;
    int8_t accelDir;
    uint32_t maxSPS;
    uint8_t stepPort;
    uint8_t stepPin;
    uint8_t dirPort;
    uint8_t dirPin;
    int8_t dir; // 0 -> not set yet, -1 -> low, 1 -> high
    int32_t currPos; // position relative to target. 0 when at target
public:
    StepperMotor(uint8_t stepPort, uint8_t stepPin, uint8_t dirPort, uint8_t dirPin);

    void update();

    // if steps < 0, set dir low. if steps < 0, set dir high
    void step(int32_t steps);
    void setMaxStepsPerSec(uint32_t sps);
    void setAcceleration(float accel);
    bool reachedTarget() const;
};

#endif