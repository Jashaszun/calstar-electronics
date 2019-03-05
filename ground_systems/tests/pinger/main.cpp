/*
 * CalSTAR Avionics Ground Station Test: Pinger
 *
 * file: main.cpp
 *
 * Sends out the string "wiuwlehfliaw" at 0.5 second intervals, and puts the
 *    char 'h' over UART when it does so.
 * Use to verify that the Ground Station can send properly.
 */

#include "RFM69/RFM69.hpp"
#include "mbed.h"

#define NODE_ID (11)
#define NETWORK_ID (100)

#define RADIO_INT (PC_0)
#define SPI1_MOSI (PA_7)
#define SPI1_MISO (PA_6)
#define SPI1_SCLK (PA_5)
#define SPI1_SSEL (PA_4)
#define RADIO_RST (PC_1)

#define FREQUENCY (RF69_433MHZ)
#define TRANSMIT_TO (255)

#define IS_RFM69HCW
#define ENCRYPT_KEY "sampleEncryptKey"

#define BAUDRATE (115200)
#define UART6_RX (PC_7)
#define UART6_TX (PC_6)

#define PORT1 (PB_5)
#define PORT2 (PB_6)
#define PORT3 (PB_8)
#define PORT4 (PB_7)

int main() {

  DigitalOut port1(PORT1);
  port1 = 0;
  DigitalOut port2(PORT2);
  port2 = 1;
  DigitalOut port4(PORT4);
  port4 = 0;

  DigitalOut dio1(PB_12);
  dio1 = 1;
  DigitalOut dio2(PB_13);
  dio2 = 0;

  Serial pc(UART6_TX, UART6_RX);
  pc.baud(BAUDRATE);
  pc.set_blocking(false);

  RFM69 radio(SPI1_MOSI, SPI1_MISO, SPI1_SCLK, SPI1_SSEL, RADIO_RST, true);
  radio.reset();
  pc.printf("radio reset\r\n");

  radio.init();
  pc.printf("radio init'd\r\n");

  radio.setAESEncryption("sampleEncryptKey", strlen("sampleEncryptKey"));
  radio.sleep();

  radio.setPowerDBm(10);
  pc.printf("radio high powered\r\n");

  radio.setCSMA(true);

  char rx[64];
  while (true) {
    radio.send("wiuwlehfliaw", strlen("wiuwlehfliaw"));
    radio.sleep();
    pc.putc('h');
    wait(0.5);
  }
  return 0;
}
