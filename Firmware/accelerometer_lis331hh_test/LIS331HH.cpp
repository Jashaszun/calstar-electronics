#include "LIS331HH.h"

#include <Wire.h>

LIS331HH::LIS331HH(const bool lesser_bit):
    addr(LIS331HH_BASE_ADDRESS|lesser_bit)
{
    Wire.begin();
}

int16_t LIS331HH::get_x() const {
    return (read_reg(LIS331HH_OUT_X_H) << 8) | read_reg(LIS331HH_OUT_X_L);
}

int16_t LIS331HH::get_y() const {
    return (read_reg(LIS331HH_OUT_Y_H) << 8) | read_reg(LIS331HH_OUT_Y_L);
}

int16_t LIS331HH::get_z() const {
    return (read_reg(LIS331HH_OUT_Z_H) << 8) | read_reg(LIS331HH_OUT_Z_L);
}

byte LIS331HH::read_reg(const byte reg) const {
    Wire.beginTransmission(addr);
    Wire.write(reg);
    Wire.endTransmission(false);

    Wire.requestFrom(addr, (byte)1);

    byte delay = 0;
    while ((Wire.available() <= 0) && delay < LIS331HH_READ_TIMEOUT) {
        ++delay;
    }

    if (delay < LIS331HH_READ_TIMEOUT) {
        return Wire.read();
    } else {
        return -1;
    }
}

void LIS331HH::write_reg(const byte reg, const byte val) const {
    Wire.beginTransmission(addr);
    Wire.write(reg);
    Wire.write(val);
    Wire.endTransmission(true);
}

byte LIS331HH::get_i2c_addr() const {
    return addr;
}