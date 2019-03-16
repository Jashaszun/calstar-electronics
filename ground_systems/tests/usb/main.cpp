#include "mbed.h"

#include "USBSerial.h"

int main() {
    USBSerial pc;
    while (true) {
        pc.printf("Hello this is board\r\n");
        wait_ms(50);
    }
    return 0;
}
