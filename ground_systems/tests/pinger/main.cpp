/*
 * CalSTAR Avionics Ground Station Test: Pinger
 *
 * file: main.cpp
 *
 * Sends out a string at 0.5 second intervals, and puts the
 *    char 'h' over UART when it does so.
 * Use to verify that the Ground Station can send properly.
 */

#include "RFM69/RFM69.hpp"
#include "mbed.h"

#define SPI1_MOSI (PA_7)
#define SPI1_MISO (PA_6)
#define SPI1_SCLK (PA_5)
#define SPI1_SSEL (PA_4)
#define RADIO_RST (PC_1)

#define ENCRYPT_KEY ("CALSTARENCRYPTKE")

#define BAUDRATE (115200)

#define UART6_RX (PC_7)
#define UART6_TX (PC_6)

int main() {
        
  Serial pc(UART6_TX, UART6_RX);
  pc.baud(BAUDRATE);
  pc.set_blocking(false);

  RFM69 radio(SPI1_MOSI, SPI1_MISO, SPI1_SCLK, SPI1_SSEL, RADIO_RST, true);
  radio.reset();
  pc.printf("radio reset\r\n");

  radio.init();
  pc.printf("radio init'd\r\n");

  radio.setAESEncryption(ENCRYPT_KEY, strlen(ENCRYPT_KEY));
  radio.sleep();

  radio.setPowerDBm(10);
  pc.printf("radio high powered\r\n");

  radio.setCSMA(true);

  while (true) {
    radio.send("abcdefghijklmnopqrstuvwxyz0123456789", strlen("abcdefghijklmnopqrstuvwxyz0123456789"));
    radio.sleep();
    pc.putc('h');
    wait(0.5);
  }
  return 0;
}
