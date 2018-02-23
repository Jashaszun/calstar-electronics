#include <stdint.h>
#include "String.h"

#define _SERIAL_BUF_SIZE 64
#define _SERIAL_DEFAULT_TIMEOUT_MS 1000

class _Serial {
private:

    class _CircQueue {
    public:
        uint8_t buf[_SERIAL_BUF_SIZE];
        uint8_t size;
        uint8_t nextFirst;
        uint8_t nextLast;
        _CircQueue();
        bool full() const;
        bool empty() const;
        bool putFirst(uint8_t byte);
        bool putLast(uint8_t byte);
        int16_t popFirst();
        int16_t popLast();
        int16_t peekFirst() const;
        int16_t peekLast() const;
        uint8_t size() const;
    };

    _CircQueue rxBuf;
    _CircQueue txBuf;

    uint32_t timeout;
public:
    _Serial();

    void begin(int baudRate);

    uint8_t available() const;

    int16_t peek() const;

    // Returns a single byte
    int16_t readByte();

    // Blocks, until either timeout or len number of bytes are read
    uint8_t readBytes(uint8_t *buf, uint8_t len);

    void write(uint8_t byte);

    uint8_t write(const uint8_t *buf, uint8_t len);

    void print(const String &str);

    void setTimeout(uint32_t timeout);

    // pls no use
    void _rx_isr_();
    void _tx_isr_();
};
_Serial Serial;