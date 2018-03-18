#include <Arduino.h>
#include <AccelStepper.h>

#define STEP 2
#define DIR 3

// #include <RFM69.h>

// #define FREQUENCY RF69_433MHZ
// #define NODEID (4)
// #define NETWORKID (100)
// #define TRANSMIT_TO (255) // broadcast
// #define FREQUENCY RF69_433MHZ
// #define ENCRYPTKEY "sampleEncryptKey"

// RFM69 radio;
bool command_available = false;
String command = "";
void resetCommand();
void sendMessage(String message);
void moveStepperMotor(char motor, int32_t targetAngle);
int32_t angleToSteps(int32_t angle);

AccelStepper s(AccelStepper::DRIVER, STEP, DIR);

int main(void) {
    init();
    Serial.begin(19200);
    s.setMaxSpeed(2000.0);
    s.setAcceleration(1000.0);
    // radio.initialize(FREQUENCY, NODEID, NETWORKID);
    // radio.setHighPower();
    // radio.encrypt(ENCRYPTKEY);

    while (true) {
        s.run();
        // Read command from serial
        if (Serial.available()) {
            command_available = true;
            command = Serial.readString();
            if (command.endsWith("\n")) {
                command = command.substring(0, command.length() - 1);
            }
        }
        // Read command from radio
        // if (radio.receiveDone()) {
        //     if (radio.ACKRequested()) {
        //         radio.sendACK();
        //     }
        //     command_available = true;
        //     command = "";
        //     for (int i = 0; i < radio.DATALEN - 1; i++) {
        //         command += (char) radio.DATA[i];
        //     }
        //     Serial.print("Received command: ");
        //     Serial.println(command);
        // }
        // Commands should be in format "<motor char> <+ or -><2 digit number>"
        // eg: "a -90" moves motor a to -90 position
        if (command.length() >= 5) {
            sendMessage("Command recieved: " + command);
            char motor = command[0];
            int n = atoi(command.substring(2).c_str());
            // Motors are designed by a char from a to c
            if (motor < 'a' || motor > 'c') {
                sendMessage("Received invalid message: " + command);
            } else {
                moveStepperMotor(motor, n);
            }
            command = "";
        } else {
            if (command != "") {
                sendMessage("Received invalid message: " + command);  
            }    
            command = "";      
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
            s.move(angleToSteps(targetAngle));
            break;
        case 'b':
            break;
        case 'c':
            break;
        // TODO
    }
}

int32_t angleToSteps(int32_t angle) {
    return (angle*1000)/360;
}
