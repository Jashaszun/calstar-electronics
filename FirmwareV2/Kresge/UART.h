#include <stdint.h>
#include "String.h"

#define _SERIAL_BUF_SIZE 64

class _Serial {
private:

    class _Stack {
    public:
        uint8_t buf[_SERIAL_BUF_SIZE];
        uint8_t nextTop;
        _Stack();
        bool full() const;
        bool empty() const;
        bool put(uint8_t byte);
        uint8_t pop();
        uint8_t peek() const;
        uint8_t size() const;
    };

    _Stack rxBuf;
    _Stack txBuf;
public:
    void begin(int baudRate);

    uint8_t available() const;

    uint8_t peek() const;

    // Returns a single byte
    uint8_t readByte();

    // Places len number of bytes into buf, and returns number of bytes placed into buf.
    uint8_t readBytes(uint8_t *buf, uint8_t len);

    void write(uint8_t byte);

    uint8_t write(const uint8_t *buf, uint8_t len);

    void print(const String &str);

    // pls no use
    void _rx_isr_();
    void _tx_isr_();
};
_Serial Serial;