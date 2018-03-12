#include "StepperMotor.h"
#include "Timer.h"
#include "DIO.h"

StepperMotor::StepperMotor(uint8_t stepPort, uint8_t stepPin, uint8_t dirPort, uint8_t dirPin) {

}

void StepperMotor::update() {

}

void StepperMotor::step(int32_t steps) {
    if (dir == 0 && steps == 0) {
        // do nothing
    } else if (dir == 0 || reachedTarget()) {
        // set dir to sign of steps
        if (steps > 0) {
            dir = 1;
            accelDir = 1;
            digitalWrite(dirPort, dirPin, HIGH);
        } else if (steps < 0) {
            dir = -1;
            accelDir = -1;
            digitalWrite(dirPort, dirPin, LOW);
        }
        currPos = -steps;
    } else if (steps * dir < 0) { // currently moving in the opposite direction of new target
        if (steps > 0) {
            accelDir = 1;
        } else if (steps < 0) {
            accelDir = -1;
        }
        currPos = -steps;
    } else if (steps * dir > 0) { // currently moving in same direction of new target
        currPos = -steps;
    } else if (steps == 0) { // target where currently is
        currPos = 0;

    }
}

void StepperMotor::setMaxStepsPerSec(uint32_t sps) {
    maxSPS = sps;
}

void StepperMotor::setAcceleration(float accel) {
    this->accel = accel;
}

bool StepperMotor::reachedTarget() const {
    return currPos == 0;
}
