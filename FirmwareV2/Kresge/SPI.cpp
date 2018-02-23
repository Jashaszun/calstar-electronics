#include "SPI.h"

typedef struct Slave {
  char port;
  char pin;
} Slave;

short SPI_init(short clock_div) { // Initializes SPI interface with clock speed f_osc / clock_div
  SPCR = (1 << SPE) | (1 << MSTR);
  switch (clock_div) {
    case 2:
      SPSR |= (1 << SPI2X);
      return 1;
    case 4:
      return 1;
    case 8:
      SPSR |= (1 << SPI2X);
      SPCR |= (1 << SPR0);
      return 1;
    case 16:
      SPCR |= (1 << SPR0);
      return 1;
    case 32:
      SPSR |= (1 << SPI2X);
      SPCR |= (1 << SPR1);
      return 1;
    case 64:
      SPCR |= (1 << SPR1);
      return 1;
    case 128:
      SPCR |= (1 << SPR0) | (1 << SPR1);
      return 1;
    default:
      SPCR &= ~(1 << SPE);
      return 0;
  }
}

char read_data(Slave slave) {
  digitalWrite(slave.port, slave.pin, LOW); // activate slave
  while (!(SPSR & (1 << SPIF)));
  char rv = SPDR;
  digitalWrite(slave.port, slave.pin, HIGH); // deactivate slave
  return rv;
}

void write_data(Slave slave, char t_byte) {
  SPDR = t_byte;
  digitalWrite(slave.port, slave.pin, LOW); // activate slave
  while (!(SPSR & (1 << SPIF)));
  char rv = SPDR;
  digitalWrite(slave.port, slave.pin, HIGH); // deactivate slave
}
