#include <mbed.h>

#define UART6_RX    (PA_3)
#define UART6_TX    (PA_2)

int main() {
    Serial pc(UART6_TX, UART6_RX);
    pc.baud(9600);

    DigitalOut dio1(PB_15);
    DigitalOut dio2(PB_13);
    dio1=1;
    dio2=0;

    while (true) {
        pc.putc('h');
        if (pc.readable())
            pc.putc(pc.getc());
    }
    return 0;
}
