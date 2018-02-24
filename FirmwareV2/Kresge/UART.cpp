#include "UART.h"

_Serial::_CircQueue::_CircQueue():
    nextFirst(0),
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

bool _Serial::_CircQueue::putFirst(uint8_t byte) {
    if (full()) {
        return false;
    } else {
        ++size;
        buf[nextFirst] = byte;
        nextFirst = decrement(nextFirst);
        return true;
    }
}

bool _Serial::_CircQueue::putLast(uint8_t byte) {
    if (full()) {
        return false;
    } else {
        ++size;
        buf[nextLast] = byte;
        nextLast = increment(nextLast);
        return true;
    }
}

int16_t _Serial::_CircQueue::popFirst() {
    if (empty()) {
        return -1;
    } else {
        --size;
        nextFirst = increment(nextFirst);
        return buf[nextFirst];
    }
}

int16_t _Serial::_CircQueue::popLast() {
    if (empty()) {
        return -1;
    } else {
        --size;
        nextLast = decrement(nextLast);
        return buf[nextLast];
    }
}

int16_t _Serial::_CircQueue::peekFirst() const {
    if (empty()) {
        return -1;
    } else {
        return buf[increment(nextFirst)];
    }
}

int16_t _Serial::_CircQueue::peekLast() const {
    if (empty()) {
        return -1;
    } else {
        return buf[decrement(nextLast)];
    }
}

_Serial::_Serial():
    timeout(_SERIAL_DEFAULT_TIMEOUT_MS) {

}

void _Serial::begin(int baudRate) {
    cli(); // disable interrupts

    timeout = _SERIAL_DEFAULT_TIMEOUT_MS;

    UCSR0B |= (1 << RXCIE0) | (1 << TXCIE0); // enable interrupts on uart recieve and transmit lines
    UCSR0B |= (1 << RXEN0) | (1 << TXEN0); // enable transmission and reception

    UCSR0C |= (1 << UCSZ01) | (1 << UCSZ00); // frame format: 8 bis, 1 stop bit, no parity

    // Set baud value
    uint16_t baudValue = ((((F_CPU / 16) + (baudRate / 2)) / (baudRate)) - 1);
    UBRR0H = (uint8_t) (baudValue >> 8);
    UBRR0L = (uint8_t) baudValue;

    sei(); // re-enable interrupts
}

uint8_t _Serial::available() const {
    return rxBuf.size;
}

bool _Serial::availableForWrite() const {
    return UCSR0A & (1 << UDRE0); // returns whether transmission buffer is empty
}

int16_t _Serial::peek() const {
    return rxBuf.peekFirst();
}

int16_t _Serial::readByte() {
    return rxBuf.popFirst();
}

uint8_t min(uint8_t a, uint8_t b) {
    if (a < b) {
        return a;
    } else {
        return b;
    }
}

uint8_t _Serial::readBytes(uint8_t *buf, uint8_t len) {
    // time(0) and such return time in seconds. need Timer class implemented before this
    // can be completed


    // time_t last_recieved = time(0);
    // uint8_t i = 0;
    // while (i != len) {
    //     if (available() > 0) {
    //         buf[i] = readByte();
    //         ++i;
    //         last_recieved = time(0);
    //     }
    //     if (difftime(time(0), last_recieved) > timeout) {
    //         break;
    //     }
    // }
    // return i;
    return 0;
}

void _Serial::write(uint8_t byte) {
    if (availableForWrite()) {
        UDR0 = byte;
    } else {
        txBuf.putLast(byte);
    }
}

void _Serial::write(const uint8_t *buf, uint8_t len) {
    for (uint8_t i = 0; i < len; ++i) {
        write(buf[i]);
    }
}

void _Serial::print(const String &str) {
    write((uint8_t *)str.const_c_str(), str.length());
}

void _Serial::println(const String &str) {
    print(str + "\n");
}

void _Serial::setTimeout(uint32_t timeout) {
    timeout = timeout;
}

// place byte at end of rx buffer
void _Serial::_rx_isr_() {
    rxBuf.putLast(UDR0);
}

// send next first byte of tx buffer
void _Serial::_tx_isr_() {
    if (!txBuf.empty()) {
        UDR0 = txBuf.popFirst();
    }
}

// these vector names are specifically for the ATmega328P.
// For other devices, should probably do #defining in the .h to make everything uniform
ISR(USART_TX_vect, ISR_BLOCK) {
    Serial._tx_isr_();
}

ISR(USART_RX_vect, ISR_BLOCK) {
    Serial._rx_isr_();
}

_Serial Serial;
