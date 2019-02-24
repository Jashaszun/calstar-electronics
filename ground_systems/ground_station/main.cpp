/*
 * CalSTAR Avionics Ground Station
 * 
 * file: main.cpp
 * 
 * Provides communication between laptop and rocket.
 * Communicates with laptop via UART over USB or just UART
 * Communicates with rocker via radio
 * Has 2 configurable LEDs and 3 digital inputs on terminal blocks
 *
 * LEDs:
 *  - Rx (PB12)
 *  - Tx (PB13)
 * 
 * Digital Input Ports:
 *  - Port 1 (PB5)
 *  - Port 2 (PB6)
 *  - Port 3 (PB8) // NOTE: on v3.0 CANNOT USE -- is on BOOT0 instead of a DIO pin. Can only use on v3.1+
 *  - Port 4 (PB7)
 */

#include "mbed.h"
#include "RFM69/RFM69.h"
#include <string>

#define NODE_ID     (11)
#define NETWORK_ID  (100)
#define RADIO_INT   (PC_0)
#define SPI1_MOSI   (PA_7)
#define SPI1_MISO   (PA_6)
#define SPI1_SCLK   (PA_5)
#define SPI1_SSEL   (PA_4)
#define RADIO_RST   (PC_1)
#define FREQUENCY   (RF69_433MHZ)
#define TRANSMIT_TO (255)

#define IS_RFM69HCW

#define ENCRYPT_KEY "sampleEncryptKey"//("CALSTARENCRYPTKE") // random 16 bytes that must be the same across all nodes

#define COMMAND_YES_RETRY ("![YES_RETRY]!")
#define COMMAND_NO_RETRY  ("![NO_RETRY]!")

#define BAUDRATE    (115200)

// These are for if USB fails
#define UART6_RX    (PC_7)
#define UART6_TX    (PC_6)

#define IO1 (PB_5)
#define IO2 (PB_6)
#define IO3 (PB_8)
#define IO4 (PB_7)

#define LED_RX (PB_12)
#define LED_TX (PB_13)

int main() {
    DigitalIn  io1(IO1);
    DigitalIn  io2(IO2);
    DigitalIn  io3(IO3);
    DigitalIn  io4(IO4);

    DigitalOut rx_led(LED_RX);
    rx_led = 0;
    DigitalOut tx_led(LED_TX);
    tx_led = 0;

    Serial pc(UART6_TX, UART6_RX);
    pc.baud(BAUDRATE);

    RFM69 radio(SPI1_MOSI, SPI1_MISO, SPI1_SCLK, SPI1_SSEL, RADIO_INT); 

    DigitalOut radio_rst(RADIO_RST);
    pc.printf("![Resetting radio...]!\n")
    radio_rst = 1;
    wait(0.5);
    radio_rst = 0;
    wait (0.5);
    pc.printf("![Radio reset complete.]!\n");

    radio.initialize(FREQUENCY, NODE_ID, NETWORK_ID);
    radio.encrypt(ENCRYPT_KEY);

#ifdef IS_RFM69HCW
    radio.setHighPower();
#endif

    std::string line = "";
    bool retry = false;
    while (true) {
        rx_led = 0;
        tx_led = 0;

        if (pc.readable()) {
            char in = pc.getc();
            if (in == '\n') {
                if (line == COMMAND_YES_RETRY) {
                    retry = true;
                    pc.write((const uint8_t *)line.c_str(), line.length(), NULL); 
                } else if (line == COMMAND_NO_RETRY) {
                    retry = false;
                    pc.write((const uint8_t *)line.c_str(), line.length(), NULL); 
                } else {
                    if (retry) {
                        pc.printf("![SENDING W/ RETRY '%s']!\n", line.c_str());
                        line += '\n';
                        radio.sendWithRetry(TRANSMIT_TO, line.c_str(), line.length());
                    } else {
                        pc.printf("![SENDING ONCE ' %s ']!\n", line.c_str());
                        line += '\n';
                        radio.send(TRANSMIT_TO, line.c_str(), line.length());
                    } 
                }
                line = "";
            } else {
                line += in;
                rx_led = 1;
                tx_led = 1;
            } 
        }

        if (radio.receiveDone()) {
            rx_led = 1;
            pc.printf("![RSSI=%d]!", radio.RSSI);
            pc.write((const uint8_t *)radio.DATA, radio.DATALEN, NULL); // last arg (NULL) is the tx complete callback
            pc.printf("\n");
            if (radio.ACKRequested()) {
                tx_led = 1;
                radio.sendACK();
            }
        }   
    }
    return 0;
}
