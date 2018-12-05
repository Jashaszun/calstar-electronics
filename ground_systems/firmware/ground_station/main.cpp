/*
 * CalSTAR Avionics Ground Station
 * 
 * file: main.cpp
 * 
 * Provides communication between laptop and rocket.
 * Communicates with laptop via UART over USB
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
 *  - Port 3 CANNOT USE -- is on BOOT0 instead of a DIO pin
 *  - Port 4 (PB7)
 */

#include "mbed.h"
#include "RFM69/RFM69.h"
#include <string>

#define GATEWAY_ID  (1)
#define NODE_ID     (11)
#define NETWORK_ID  (101)
#define RADIO_INT   (PC_0)
#define SPI1_MOSI   (PA_7)
#define SPI1_MISO   (PA_6)
#define SPI1_SCLK   (PA_5)
#define SPI1_SSEL   (PA_4)
#define FREQUENCY   (RF69_433MHZ)
#define TRANSMIT_TO (12)

#define IS_RFM69HCW

#define ENCRYPT_KEY ("CALSTARENCRYPTKE") // random 16 bytes that must be the same across all nodes

#define COMMAND_YES_RETRY ("![YES_RETRY]!")
#define COMMAND_NO_RETRY  ("![NO_RETRY]!")

#define BAUDRATE    (115200)

// These are for if USB fails
#define UART6_RX    (PC_7)
#define UART6_TX    (PC_6)

#define PORT1 (PB_5)
#define PORT2 (PB_6)
#define PORT4 (PB_7)

#define LED_RX (PB_12)
#define LED_TX (PB_13)

int main() {
    DigitalIn  port1(PORT1);
    DigitalIn  port2(PORT2);
    DigitalIn  port4(PORT4);

    DigitalOut rx_led(LED_RX);
    rx_led = 1;
    DigitalOut tx_led(LED_TX);
    tx_led = 0;

    Serial pc(UART6_TX, UART6_RX);
    pc.baud(BAUDRATE);
    pc.set_blocking(false);

    RFM69 radio(SPI1_MOSI, SPI1_MISO, SPI1_SCLK, SPI1_SSEL, RADIO_INT); 
    radio.initialize(FREQUENCY, NODE_ID, NETWORK_ID);
    radio.encrypt(ENCRYPT_KEY);

#ifdef IS_RFM69HCW
    radio.setHighPower();
#endif

    std::string line = "";
    bool retry = false;
    while (true) {
        //rx_led = 0;
        //tx_led = 0;
        //pc.write((const uint8_t *) "beginning loop\r\n", sizeof("beginning loop\r\n"), NULL);

        if (pc.readable()) {
            pc.write((const uint8_t *) "pc readable\r\n", sizeof("pc readable\r\n"), NULL);
            char in = pc.getc();
            if (in == '\n') {
                pc.write((const uint8_t *) "end line\r\n", sizeof("end line\r\n"), NULL);
                if (line == COMMAND_YES_RETRY) {
                    retry = true;
                    pc.write((const uint8_t *)line.c_str(), line.length(), NULL); 
                } else if (line == COMMAND_NO_RETRY) {
                    retry = false;
                    pc.write((const uint8_t *)line.c_str(), line.length(), NULL); 
                } else {
                    pc.write((const uint8_t *) "send message\r\n", sizeof("send message\r\n"), NULL);
                    if (retry) {
                        pc.printf("![SENDING W/ RETRY ' %s ']!", line.c_str());
                        line += '\n';
                        radio.sendWithRetry(TRANSMIT_TO, line.c_str(), line.length());
                    } else {
                        pc.printf("![SENDING ONCE ' %s ']!", line.c_str());
                        line += '\n';
                        radio.send(TRANSMIT_TO, line.c_str(), line.length());
                    } 
                }
                line = "";
            } else {
                line += in;
            } 
        }

        if (radio.receiveDone()) {
            //rx_led = 1;
            pc.write((const uint8_t *)radio.DATA, radio.DATALEN, NULL); // last arg (NULL) is the tx complete callback
            if (radio.ACKRequested()) {
                //tx_led = 1;
                radio.sendACK();
            }
        }   
    }
    return 0;
}
