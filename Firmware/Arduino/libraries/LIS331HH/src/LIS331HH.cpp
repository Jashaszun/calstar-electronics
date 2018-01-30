#include "LIS331HH.h"

#include <Wire.h>

LIS331HH::LIS331HH(const bool lesser_bit):
    addr(LIS331HH_BASE_ADDRESS|lesser_bit),
    scale(LIS331HH_6G_SCALE)
{
    //Wire.begin();
}

float LIS331HH::get_x_g() const {
    return get_x_raw()*scale;
}

float LIS331HH::get_y_g() const {
    return get_y_raw()*scale;
}

float LIS331HH::get_z_g() const {
    return get_z_raw()*scale;
}

int16_t LIS331HH::get_x_raw() const {
    return (read_reg(LIS331HH_OUT_X_H) << 8) | read_reg(LIS331HH_OUT_X_L);
}

int16_t LIS331HH::get_y_raw() const {
    return (read_reg(LIS331HH_OUT_Y_H) << 8) | read_reg(LIS331HH_OUT_Y_L);
}

int16_t LIS331HH::get_z_raw() const {
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

void LIS331HH::write_reg(const byte reg, const byte val) {
    // if potentially changing scale bits, update scale member variable
    if (reg == LIS331HH_CTRL_REG4) {
        switch (val & 0b00110000) {
            case 0b00000000: // scale bits set to 0b00
                scale = LIS331HH_6G_SCALE;
                break;
            case 0b00010000: // scale bits set to 0b01
                scale = LIS331HH_12G_SCALE;
                break;
            case 0b00110000: // scale bits set to 0b11
                scale = LIS331HH_24G_SCALE;
        }
    }

    Wire.beginTransmission(addr);
    Wire.write(reg);
    Wire.write(val);
    Wire.endTransmission(true);
}

byte LIS331HH::get_i2c_addr() const {
    return addr;
}

float LIS331HH::get_scale() const {
    return scale;
}