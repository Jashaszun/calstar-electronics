#include "UART.h"
#include "Timer.h"
#include <avr/interrupt.h>
#include <avr/io.h>

_Serial::_CircQueue::_CircQueue():
    first(0),
    nextLast(0),
    size(0) {

}

bool _Serial::_CircQueue::empty() const {
    return size == 0;
}

bool _Serial::_CircQueue::full() const {
    return size == _SERIAL_BUF_SIZE;
}

uint8_t decrement(uint8_t index) {
    --index;
    if (index < 0) {
        return _SERIAL_BUF_SIZE - 1;
    } else {
        return index;
    }
}

uint8_t increment(uint8_t index) {
    return (index + 1) % _SERIAL_BUF_SIZE;
}

bool _Serial::_CircQueue::put(uint8_t byte) {
    if (full()) {
        return false;
    } else {
        ++size;
        buf[nextLast] = byte;
        nextLast = increment(nextLast);
        return true;
    }
}

int16_t _Serial::_CircQueue::pop() {
    if (empty()) {
        return -1;
    } else {
        --size;
        uint8_t val = buf[first];
        first = increment(first);
        return val;
    }
}

int16_t _Serial::_CircQueue::peek() const {
    if (empty()) {
        return -1;
    } else {
        return buf[first];
    }
}

_Serial::_Serial():
    timeout(_SERIAL_DEFAULT_TIMEOUT_MS) {

}

void _Serial::begin(int baudRate) {
    timeout = _SERIAL_DEFAULT_TIMEOUT_MS;
    uint16_t baudValue = ((((F_CPU / 16) + (baudRate / 2)) / (baudRate)) - 1);    
    
    cli(); // disable interrupts

    UCSR0B |= (1 << RXEN0) | (1 << TXEN0); // enable transmission and reception

    UCSR0C |= (1 << UCSZ01) | (1 << UCSZ00); // frame format: 8 bis, 1 stop bit, no parity

    // Set baud value
    UBRR0H = (uint8_t) (baudValue >> 8);
    UBRR0L = (uint8_t) baudValue;

    UCSR0B |= (1 << RXCIE0) | (1 << TXCIE0); // enable interrupts on uart recieve and transmit lines    

    sei(); // re-enable interrupts
}

uint8_t _Serial::available() const {
    return rxBuf.size;
}

bool _Serial::availableForWrite() const {
    return UCSR0A & (1 << UDRE0); // returns whether transmission buffer is empty
}

int16_t _Serial::peek() const {
    return rxBuf.peek();
}

int16_t _Serial::readByte() {
    uint32_t t0 = millis();
    while (rxBuf.empty()) {
        if (millis() - t0 > timeout) {
            return -1;
        }
    }
    return rxBuf.pop();
}

uint8_t _Serial::readBytes(uint8_t *buf, uint8_t len) {
    uint8_t i = 0;
    while (i != len) {
        uint8_t c = readByte();
        if (c == -1) {
            break;
        }
        buf[i] = c;
        ++i;
    }
    return i;
}

bool _Serial::write(uint8_t byte) {
    uint32_t t0 = millis();
    while (txBuf.full()) {
        if (millis() - t0 > timeout) {
            return false;
        }
    }
    txBuf.put(byte);
    if (availableForWrite()) {
        UDR0 = txBuf.pop();
    }
    return true;
}

uint8_t _Serial::write(const uint8_t *buf, uint8_t len) {
    for (uint8_t i = 0; i < len; ++i) {
        if (!write(buf[i])) {
            return i;
        }
    }
}

void _Serial::print(const String &str) {
    write((uint8_t *)str.const_c_str(), str.length());
}

void _Serial::println(const String &str) {
    print(str + "\n\r");
}

void _Serial::setTimeout(uint32_t timeout) {
    this->timeout = timeout;
}

// place byte at end of rx buffer
void _Serial::_rx_isr_() {
    rxBuf.put(UDR0);
}

// send next first byte of tx buffer
void _Serial::_tx_isr_() {
    if (!txBuf.empty()) {
        UDR0 = txBuf.pop();
    }
}

// these vector names are specifically for the ATmega328P.
// For other devices, should probably do #defining in the .h to make everything uniform
ISR(USART_TX_vect) {
    Serial._tx_isr_();
}

ISR(USART_RX_vect) {
    Serial._rx_isr_();
}

_Serial Serial;