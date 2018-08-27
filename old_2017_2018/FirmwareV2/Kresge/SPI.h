#include "DIO.h"

struct Slave;

short SPI_init(short clock_div); // Initializes SPI interface with clock speed f_osc / clock_div
char read_data(Slave slave);
void write_data(Slave slave, char t_byte);
