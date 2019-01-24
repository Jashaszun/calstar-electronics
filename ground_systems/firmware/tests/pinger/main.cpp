#include "mbed.h"
#include "RFM69/RFM69.h"

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

#define LOG(uart, str) uart.write((const uint8_t *)str, strlen(str), NULL);

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
 //   pc.set_blocking(true);

    pc.printf("Hello program begin\r\n");

    RFM69 radio(SPI1_MOSI, SPI1_MISO, SPI1_SCLK, SPI1_SSEL, RADIO_INT);
    pc.printf("raio cstor'd\r\n");
    DigitalOut radio_rst(RADIO_RST);
    radio_rst = 1;
    wait(0.5);
    radio_rst = 0;
    wait(0.5);
    pc.printf("radio reset\r\n");

    radio.initialize(FREQUENCY, NODE_ID, NETWORK_ID);
    pc.printf("radio init'd\r\n");
    radio.encrypt(ENCRYPT_KEY);
    pc.printf("radio encrypted\r\n");
#ifdef IS_RFM69HCW
    //radio.setHighPower();
#endif
    pc.printf("radio high powered\r\n");

    while (true) {
//        radio.send(TRANSMIT_TO, "wiuwlehfliaw", strlen("wiuwlehfliaw"));
//        LOG(pc, "sending\n");
        pc.putc('h');
        wait(0.5);
    }

    return 0;
}
