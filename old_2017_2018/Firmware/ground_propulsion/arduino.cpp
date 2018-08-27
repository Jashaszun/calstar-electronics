#include <Arduino.h>
#include <AccelStepper.h>

#define A_STEP 3
#define A_DIR  5

#define B_STEP 7
#define B_DIR  8

#define C_STEP 10
#define C_DIR  12 

#define BAUD_RATE 19200

#define MAX_SPEED 1000.0
#define ACCEL     1000.0

#define ENABLE  "enable"
#define DISABLE "disable"
#define ALIAS   "alias"

#define A 'a'
#define B 'b'
#define C 'c'

enum State {
    ENABLED,
    DISABLED
};

//takes new line as end of entered command
bool completingCommand = false;
String command = "";
State state = ENABLED;

AccelStepper a(AccelStepper::DRIVER, A_STEP, A_DIR);
String a_alias = "";

AccelStepper b(AccelStepper::DRIVER, B_STEP, B_DIR);
String b_alias = "";

AccelStepper c(AccelStepper::DRIVER, C_STEP, C_DIR);
String c_alias = "";

void resetCommand();
void sendMessage(String message);
bool validNumber(String s);
void moveStepperMotor(char motor, int32_t targetAngle);
int32_t angleToSteps(int32_t angle);
void initStepperMotors();
void updateMotors();
void readSerialCommand();

int main(void) {
    init();
    Serial.begin(BAUD_RATE);
    initStepperMotors();

    while (true) {
        switch (state) {
            case ENABLED:
                updateMotors();
                if (completingCommand) {
                    command.trim();
                    sendMessage("Command Recieved: " + command);
                    int firstSpace = command.indexOf(' ');
                    if (firstSpace == -1) {
                        if (command == DISABLE) {
                            sendMessage("Moving from enabled state to disabled state");
                            state = DISABLED;
                        } else {
                            sendMessage("Invalid Command: " + command);
                        }
                        resetCommand();
                        break;
                    }
                    String firstArg = command.substring(0, firstSpace);
                    if (firstArg.length() == 1 && (firstArg[0] == A || firstArg[0] == B || firstArg[0] == C)) { // expects a single number next as absolute position to which to move motor to
                        String rest = command.substring(firstSpace + 1);
                        if (validNumber(rest)) {
                            int32_t n = atol(command.substring(firstSpace + 1).c_str());
                            sendMessage("Moving motor " + firstArg + " to absolute position " + n);
                            moveStepperMotor(firstArg[0], n);
                        } else {
                            sendMessage("Invalid position for motor: " + rest);
                            sendMessage("Expects an integer.");
                            resetCommand();
                            break;
                        }
                    } else if (firstArg == ALIAS) {
                        sendMessage("Aliasing not implemented");
                    } else {
                        sendMessage("Invalid Command: " + command);
                        resetCommand();
                        break;
                    }
                    resetCommand();
                }
                break;
            case DISABLED:
                if (completingCommand) {
                    command.trim();
                    if (command == ENABLE) {
                        state = ENABLED;
                        sendMessage("Moving from disabled state to enabled");
                    } else {
                        sendMessage("Invalid Command: " + command);
                        sendMessage("Only command message in disabled state is \'enable\'");
                    }
                    resetCommand();
                }
                break;
        }
        // Read command from serial
        if (!completingCommand) {
            readSerialCommand();
        }
    }
}

void sendMessage(String message) {
    Serial.println(message);
}

bool validNumber(String s) {
    if (s.length() == 0) {
        return false;
    }
    if (!(isdigit(s[0]) || s[0] == '-')) {
        return false;
    }
    for (int i = 1; i < s.length(); ++i) {
        if (!isdigit(s[i])) {
            return false;
        }
    }
    return true;
}

void moveStepperMotor(char motor, int32_t targetAngle) {
    switch (motor) {
        case A:
            a.moveTo(angleToSteps(targetAngle));
            break;
        case B:
            b.moveTo(angleToSteps(targetAngle));
            break;
        case C:
            c.moveTo(angleToSteps(targetAngle));
            break;
    }
}

int32_t angleToSteps(int32_t angle) {
    return (angle*2000)/360;
}

void initStepperMotors() {
    a.setMaxSpeed(MAX_SPEED);
    a.setAcceleration(ACCEL);

    b.setMaxSpeed(MAX_SPEED);
    b.setAcceleration(ACCEL);

    c.setMaxSpeed(MAX_SPEED);
    c.setAcceleration(ACCEL);
}

void updateMotors() {
    a.run();
    b.run();
    c.run();
}

void readSerialCommand() {
    while (Serial.available()) {
        char byte = Serial.read();
        if (byte == '\n' || byte == '\r') {
            completingCommand = true;
            break;
        }
        command += byte;
        if (state != DISABLED) {
            updateMotors();
        }
    }
}

void resetCommand() {
    completingCommand = false;
    command = "";
}