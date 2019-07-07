/*
 * CalSTAR Avionics Ground Station Test: DIO
 * 
 * file: main.cpp
 * 
 * Iterates through each broken out DIO (IO1-4, RX LED, TX LED)
 *  and turns on each for 1 second.
 * Use to verify that DIO work.
 */

#include "mbed.h"

#define IO1 (PB_5)
#define IO2 (PB_6)
#define IO3 (PB_8)
#define IO4 (PB_7)

#define LED_RX (PB_12)
#define LED_TX (PB_13)

int main() {
    DigitalOut  io1(IO1);
    io1 = 0;
    DigitalOut  io2(IO2);
    io2 = 0;
    DigitalOut  io3(IO3);
    io3 = 0;
    DigitalOut  io4(IO4);
    io4 = 0;

    DigitalOut rx_led(LED_RX);
    rx_led = 0;
    DigitalOut tx_led(LED_TX);
    tx_led = 0;

    while (true) {
        io1 = 1;
        wait(1);
        io1 = 0;
        io2 = 1;
        wait(1);
        io2 = 0;
        io3 = 1;
        wait(1);
        io3 = 0;
        io4 = 1;
        wait(1);
        io4 = 0;
        rx_led = 1;
        wait(1);
        rx_led = 0;
        tx_led = 1;
        wait(1);
        tx_led = 0;
    }
}