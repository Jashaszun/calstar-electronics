/*
 * CalSTAR Avionics Ground Station Test: Echo
 * 
 * file: main.cpp
 * 
 * Reads from UART and responds with an 'r' and then the recieved character.
 * Upon recieve will also toggle LED_RX
 * Use to verify UART tx from board works.
 */

#include "mbed.h"

#define LED_RX      (PB_12)

#define UART6_RX    (PC_7)
#define UART6_TX    (PC_6)
#define BAUDRATE    (115200)

int main() {
    DigitalOut rx_led(LED_RX);
    rx_led = 0;

    Serial pc(UART6_TX, UART6_RX);
    pc.baud(BAUDRATE);
    pc.set_blocking(false);

    while (true) {
        if (pc.readable()) {
            pc.putc('r');
            pc.putc(pc.getc());
            if (rx_led.read() == 0) {
                rx_led = 1;
            } else {
                rx_led = 0;
            }
        }
    }
    return 0;
}
