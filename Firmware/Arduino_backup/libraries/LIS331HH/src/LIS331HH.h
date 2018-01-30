/*
Not actually an external Arduino library
Written by CalSTAR 2017
*/

#ifndef LIS331HH_H
#define LIS331HH_H

#include <Arduino.h>

#define LIS331HH_READ_TIMEOUT 50

// scale = (upperbound - lowerbound)/(2^16)
#define LIS331HH_6G_SCALE  0.00018310546875f
#define LIS331HH_12G_SCALE 0.0003662109375f
#define LIS331HH_24G_SCALE 0.000732421875f

// must be bit or'd with a 0 or a 1 to result in the actual device address
#define LIS331HH_BASE_ADDRESS     0b0011000

#define LIS331HH_CTRL_REG1        0x20
#define LIS331HH_CTRL_REG2        0x21
#define LIS331HH_CTRL_REG3        0x22
#define LIS331HH_CTRL_REG4        0x23
#define LIS331HH_CTRL_REG5        0x24

#define LIS331HH_HP_FILTER_RESET 0x25
#define LIS331HH_REFERENCE       0x26
#define LIS331HH_STATUS_REG      0x27

#define LIS331HH_OUT_X_L 0x28
#define LIS331HH_OUT_X_H 0x29

#define LIS331HH_OUT_Y_L 0x2A
#define LIS331HH_OUT_Y_H 0x2B

#define LIS331HH_OUT_Z_L 0x2C
#define LIS331HH_OUT_Z_H 0x2D

#define LIS331HH_INT1_CFG        0x30
#define LIS331HH_INT1_SOURCE     0x31
#define LIS331HH_INT1_THS        0x32
#define LIS331HH_INT1_DURATION   0x33

#define LIS331HH_INT2_CFG        0x34
#define LIS331HH_INT2_SOURCE     0x35
#define LIS331HH_INT2_THS        0x36
#define LIS331HH_INT2_DURATION   0x37

class LIS331HH {
private:
    byte addr;
    float scale;
public:
    LIS331HH(const bool lesser_bit);

    float get_x_g() const;
    float get_y_g() const;
    float get_z_g() const;

    int16_t get_x_raw() const;
    int16_t get_y_raw() const;
    int16_t get_z_raw() const;

    byte read_reg(const byte reg) const;
    void write_reg(const byte reg, const byte val);

    byte get_i2c_addr() const;
    float get_scale() const;

};

#endif