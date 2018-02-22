#include "UART.h"

#include <string.h>
#include <avr/interrupt.h>

_Serial::_Stack::_Stack():
    nextTop(_SERIAL_BUF_SIZE - 1) {

}

bool _Serial::_Stack::empty() const {
    return nextTop == _SERIAL_BUF_SIZE - 1;
}

bool _Serial::_Stack::full() const {
    return nextTop == -1;
}

bool _Serial::_Stack::put(uint8_t byte) {
    if (full()) {
        return false;
    } else {
        buf[nextTop] = byte;
        --nextTop;
        return true;
    }
}

uint8_t _Serial::_Stack::pop() {
    if (empty()) {
        // idk???
    } else {
        ++nextTop;
        return buf[nextTop - 1];
    }
}

uint8_t _Serial::_Stack::peek() const {
    if (empty()) {
        // idk???
    } else {
        return buf[nextTop - 1];
    }
}

uint8_t _Serial::_Stack::size() const {
    return _SERIAL_BUF_SIZE - nextTop - 1;
}

void _Serial::begin(int baudRate) {

}

uint8_t _Serial::available() const {
    return !rxBuf.empty();
}

uint8_t _Serial::peek() const {
    return rxBuf.peek();
}

uint8_t _Serial::readByte() {
    return rxBuf.pop();
}

uint8_t min(uint8_t a, uint8_t b) {
    if (a < b) {
        return a;
    } else {
        return b;
    }
}

uint8_t _Serial::readBytes(uint8_t *buf, uint8_t len) {
    uint8_t numBytes = min(len, rxBuf.size());
    memcpy(buf, &rxBuf.buf, numBytes);
    return numBytes;
}

void _Serial::write(uint8_t byte) {

}

uint8_t _Serial::write(const uint8_t *buf, uint8_t len) {

}

void _Serial::print(const String &str) {
    write((uint8_t *)str.const_c_str(), str.length());
}

void _Serial::_rx_isr_() {

}

void _Serial::_tx_isr_() {

}

// these vector names are specifically for the ATmega328P.
// For other devices, should probably do #defining in the .h to make everything uniform
ISR(USART_TX_vect) {
    Serial._tx_isr_();
}

ISR(USART_RX_vect) {
    Serial._rx_isr_();
}