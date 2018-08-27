// Pass through program between radio and uart
// radio <--> uart

#include <Arduino.h>
#include <RFM69.h>

#define FREQUENCY RF69_433MHZ
#define NODEID (4)
#define NETWORKID (100)
#define TRANSMIT_TO (255) // broadcast
#define ENCRYPTKEY "sampleEncryptKey"

int main(void) {
    init();
    Serial.begin(19200);
    RFM69 radio;
    radio.initialize(FREQUENCY, NODEID, NETWORKID);
    radio.setHighPower();
    radio.encrypt(ENCRYPTKEY);

    while (true) {
        // Read command from serial
        if (Serial.available()) {
            String str = Serial.readString();
            radio.send(TRANSMIT_TO, str.c_str(), str.length());
        }
        //Read command from radio, send it via UART
        if (radio.receiveDone()) {
            if (radio.ACKRequested()) {
                radio.sendACK();
            }
            char buf[100];
            for (int i = 0; i < radio.DATALEN; i++) {
                buf[i] = (char) radio.DATA[i];
            }
            buf[radio.DATALEN] = '\0';
            Serial.write(buf, radio.DATALEN);
        }
    }
}