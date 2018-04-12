#include <Arduino.h>
#include <AccelStepper.h>

#define A_STEP 2
#define A_DIR 3

#define B_STEP 4
#define B_DIR 5

#define C_STEP 6
#define C_DIR 7 

//takes new line as end of entered command
bool command_available = false;
String command = "";
void resetCommand();
void sendMessage(String message);
void moveStepperMotor(char motor, int32_t targetAngle);
int32_t angleToSteps(int32_t angle);

AccelStepper a(AccelStepper::DRIVER, A_STEP, A_DIR);
AccelStepper b(AccelStepper::DRIVER, B_STEP, B_DIR);
AccelStepper c(AccelStepper::DRIVER, C_STEP, C_DIR);


int main(void) {
    init();
    Serial.begin(19200);
    a.setMaxSpeed(1000.0);
    a.setAcceleration(1000.0);
    b.setMaxSpeed(1000.0);
    b.setAcceleration(1000.0);
    c.setMaxSpeed(1000.0);
    c.setAcceleration(1000.0);

    while (true) {
        a.run();
        b.run();
        c.run();
        // Read command from serial
        if (Serial.available() && command_available == false) {
            while (Serial.available()) {
                char byte = Serial.read();
                if (byte == '\n' || byte == '\r') {
                    command_available = true;
                    break;
                }
                command += byte;
                a.run();
                b.run();
                c.run();
            }
        }
        // Commands should be in format "<motor char> <+ or -><2 digit number>"
        // eg: "a -90" moves motor a to -90 position, absolute -90, not relative (absolute -90 is relative to middle position)
        if (command_available && command.length() >= 5) {
            sendMessage("Command recieved: " + command);
            char motor = command[0];
            int n = atoi(command.substring(2).c_str());
            if (n > 90) {
                n = 90;
            } else if (n < -90) {
                n = -90;
            }
            // Motors are designated by a char from a to c
            if (motor < 'a' || motor > 'c') {
                sendMessage("Received invalid message: " + command);
            } else {
                moveStepperMotor(motor, n);
            }
            command = "";
            command_available = false;
        } else if (command_available && command.length() < 5) {
            if (command != "") {
                sendMessage("Received invalid message: " + command);  
            }    
            command = "";
            command_available = false; 
        }
    }
}

void sendMessage(String message) {
    Serial.println(message);
    // radio.send(TRANSMIT_TO, message.c_str(), message.length());
}

void moveStepperMotor(char motor, int32_t targetAngle) {
    switch (motor) {
        case 'a':
            a.moveTo(angleToSteps(targetAngle));
            break;
        case 'b':
            b.moveTo(angleToSteps(targetAngle));
            break;
        case 'c':
            c.moveTo(angleToSteps(targetAngle));
            break;
        // TODO
    }
}

int32_t angleToSteps(int32_t angle) {
    return (angle*2000)/360;
}
