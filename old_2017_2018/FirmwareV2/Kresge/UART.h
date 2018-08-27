#ifndef _KRESGE_UART_H_
#define _KRESGE_UART_H_

#include <stdint.h>
#include "KString.h"

#define _SERIAL_BUF_CAPACITY 64
#define _SERIAL_DEFAULT_TIMEOUT_MS 1000

class _Serial {
private:

    class _CircQueue {
    public:
        uint8_t buf[_SERIAL_BUF_CAPACITY];
        uint8_t size;
        uint8_t first;
        uint8_t nextLast;
        _CircQueue();
        bool full() const;
        bool empty() const;
        bool put(uint8_t byte);
        int16_t pop();
        int16_t peek() const;
    };

    _CircQueue rxBuf;
    _CircQueue txBuf;

    uint32_t timeout;

    bool txRegEmpty() const;
public:
    _Serial();

    void begin(int baudRate);

    // Returns how many bytes are in the rx buffer
    uint8_t available() const;

    // Returns how many bytes of space are free in the tx buffer
    uint8_t availableForWrite() const;

    // Returns (without removing) the first byte in the rx buffer
    int16_t peek() const;

    // Returns a single byte, -1 if no byte in rx buffer
    int16_t readByte();

    // Blocks, until either timeout or len number of bytes are read.
    // Returns number of bytes read
    uint8_t readBytes(uint8_t *buf, uint8_t len);

    // Writes byte to tx buffer. Does nothing if tx buffer is already full.
    bool writeByte(uint8_t byte);

    // Blocks, until either timeout or len number of bytes are written
    // Returns number of bytes written.
    uint8_t writeBytes(const uint8_t *buf, uint8_t len);

    void print(const String &str);

    void println(const String &str);

    void setTimeout(uint32_t timeout);

    // pls no use
    void _rx_isr_();
    void _tx_isr_();
};

extern _Serial Serial;

#endif
