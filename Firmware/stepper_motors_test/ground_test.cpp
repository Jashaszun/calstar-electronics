#include <Arduino.h>
#include <RFM69.h>

#define FREQUENCY RF69_433MHZ
#define NODEID (4)
#define NETWORKID (100)
#define TRANSMIT_TO (255) // broadcast
#define FREQUENCY RF69_433MHZ
#define ENCRYPTKEY "sampleEncryptKey"

RFM69 radio;
bool command_available = false;
String command = "";
void resetCommand();
void sendMessage(String message);
void moveStepperMotor(char motor, int angle);

int main(void) {
    init();
    Serial.begin(19200);
    radio.initialize(FREQUENCY, NODEID, NETWORKID);
    radio.setHighPower();
    radio.encrypt(ENCRYPTKEY);

    while (true) {
        // Read command from serial
        if (Serial.available()) {
            command_available = true;
            command = Serial.readString();
            if (command.endsWith("\n")) {
                command = command.substring(0, command.length() - 1);
            }
        }
        // Read command from radio
        if (radio.receiveDone()) {
            if (radio.ACKRequested()) {
                radio.sendACK();
            }
            command_available = true;
            command = "";
            for (int i = 0; i < radio.DATALEN - 1; i++) {
                command += (char) radio.DATA[i];
            }
            Serial.print("Received command: ");
            Serial.println(command);
        }
        // Longest command is "a -90", shortest is "a 3"
        if (command.length() >= 3 && command.length() <= 5) {
            char motor = command[0];
            int n = atoi(command.substring(2).c_str());
            // Motors are designed by a char from a to c
            if (motor < 'a' || motor > 'c') {
                sendMessage("Received invalid message: " + command);
                break;
            } else {
                moveStepperMotor(motor, n);
            }
        }
    }
}

void sendMessage(String message) {
    Serial.println(message);
    radio.send(TRANSMIT_TO, message.c_str(), message.length());
}

void moveStepperMotor(char motor, int angle) {
    switch (motor) {
        case 'a':
            break;
        case 'b':
            break;
        case 'c':
            break;
        // TODO
    }
}