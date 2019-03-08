/*
 * CalSTAR Avionics Ground Station
 *
 * file: main.cpp
 *
 * Provides communication between laptop and rocket.
 * Communicates with laptop via UART over USB or just UART
 * Communicates with rocker via radio
 * Has 2 configurable LEDs and 4 digital inputs on terminal blocks
 *
 * LEDs:
 *  - Rx (PB12)
 *  - Tx (PB13)
 *
 * Digital Input Ports:
 *  - Port 1 (PB5)
 *  - Port 2 (PB6)
 *  - Port 3 (PB8) // NOTE: on v3.0 CANNOT USE -- is on BOOT0 instead of a DIO
 * pin. Can only use on v3.1+
 *  - Port 4 (PB7)
 */

#include "RFM69/RFM69.hpp"
#include "mbed.h"
#include <string>

#define SPI1_MOSI (PA_7)
#define SPI1_MISO (PA_6)
#define SPI1_SCLK (PA_5)
#define SPI1_SSEL (PA_4)
#define RADIO_RST (PC_1)

// random 16 bytes that must be the same across all nodes
#define ENCRYPT_KEY ("CALSTARENCRYPTKE")

#define COMMAND_YES_RETRY ("![YES_RETRY]!")
#define COMMAND_NO_RETRY ("![NO_RETRY]!")

#define BAUDRATE (115200)

 #define UART6_RX (PA_3)
 #define UART6_TX (PA_2)

// These are for if USB fails
//#define UART6_RX (PC_7)
//#define UART6_TX (PC_6)

#define IO1 (PB_5)
#define IO2 (PB_6)
#define IO3 (PB_8)
#define IO4 (PB_7)

#define LED_RX (PB_12)
#define LED_TX (PB_13)

int main() {
  /*
    DigitalIn io1(IO1);
    DigitalIn io2(IO2);
    DigitalIn io3(IO3);
    DigitalIn io4(IO4);
  */

  DigitalOut rx_led(LED_RX);
  rx_led = 0;
  DigitalOut tx_led(LED_TX);
  tx_led = 0;

  Serial pc(UART6_TX, UART6_RX);
  pc.baud(BAUDRATE);
  pc.set_blocking(false);

  char rx_buf[128];

  RFM69 radio(SPI1_MOSI, SPI1_MISO, SPI1_SCLK, SPI1_SSEL, RADIO_RST, true);
  radio.reset();
  pc.printf("![Radio reset complete.]!\r\n");

  radio.init();
  radio.setAESEncryption(ENCRYPT_KEY, strlen(ENCRYPT_KEY));
  // radio.sleep(); // lowest power settings

  radio.setHighPowerSettings(true);
  radio.setPowerDBm(20);

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
            pc.printf("![SENDING ONCE (RETRY TODO) '%s', bytes: %d]!\r\n",
                      line.c_str(), line.length());
            line += '\n';
            radio.send(line.c_str(), line.length());
          } else {
            pc.printf("![SENDING ONCE ' %s ', bytes: %d]!\r\n", line.c_str(),
                      line.length());
            line += '\n';
            radio.send(line.c_str(), line.length());
          }
        }
        line = "";
      } else {
        line += in;
        rx_led = 1;
        tx_led = 1;
      }
    }

    int num_bytes_rxd = radio.receive(rx_buf, sizeof(rx_buf));
    if (num_bytes_rxd > 1) {
      rx_buf[num_bytes_rxd] = '\0';
      rx_led = 1;
      pc.printf("![RSSI=%d, bytes: %d]! %s\r\n", radio.getRSSI(),
                num_bytes_rxd - 1, rx_buf + 1);
      // last arg (NULL) is the tx complete callback
    }
  }
  return 0;
}
