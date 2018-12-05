#include <mbed.h>


#define LED1 (PB_12)
#define LED2 (PB_13)

#define UART6_RX    (PC_7)
#define UART6_TX    (PC_6)

int main() {
    Serial pc(UART6_TX, UART6_RX);
    pc.baud(9600);

    DigitalOut led1(LED1);
    led1 = 0;
    DigitalOut led2(LED2);
    led2 = 0;

    while (true) {
        if (pc.readable())
            pc.putc(pc.getc());
    }
    return 0;
}
