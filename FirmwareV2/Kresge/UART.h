#ifndef _KRESGE_UART_H_
#define _KRESGE_UART_H_

#include "common.h"
#include "KString.h"

#define _SERIAL_BUF_SIZE 64
#define _SERIAL_DEFAULT_TIMEOUT_MS 1000

class _Serial {
private:

    class _CircQueue {
    public:
        uint8_t buf[_SERIAL_BUF_SIZE];
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
public:
    _Serial();

    void begin(int baudRate);

    uint8_t available() const;

    bool availableForWrite() const;

    int16_t peek() const;

    // Returns a single byte
    int16_t readByte();

    // Blocks, until either timeout or len number of bytes are read
    uint8_t readBytes(uint8_t *buf, uint8_t len);

    bool write(uint8_t byte);

    uint8_t write(const uint8_t *buf, uint8_t len);

    void print(const String &str);

    void println(const String &str);

    void setTimeout(uint32_t timeout);

    // pls no use
    void _rx_isr_();
    void _tx_isr_();
};

extern _Serial Serial;

#endif
