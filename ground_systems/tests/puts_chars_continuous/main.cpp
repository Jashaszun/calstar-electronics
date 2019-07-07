/*
 * CalSTAR Avionics Ground Station Test: Puts Chars Continuous
 * 
 * file: main.cpp
 * 
 * Continuously puts the character 'h' over UART.
 * Use to verify UART tx from board works.
 */

#include "mbed.h"

#define UART6_RX    (PC_7)
#define UART6_TX    (PC_6)
#define BAUDRATE    (115200)

int main() {
    Serial pc(UART6_TX, UART6_RX);
    pc.baud(BAUDRATE);
    pc.set_blocking(false);

    while (true) {
        pc.putc('h');
    }
}
