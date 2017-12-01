/* 
 MPL3115A2 Barometric Pressure Sensor Library
 By: Nathan Seidle
 SparkFun Electronics
 Date: September 24th, 2013
 License: This code is public domain but you buy me a beer if you use this and we meet someday (Beerware license).
 
 Get pressure, altitude and temperature from the MPL3115A2 sensor.
 
 */
 
#ifndef _SPARKFUN_MPL3115A2_H_ 
#define _SPARKFUN_MPL3115A2_H_ 

#include <Arduino.h>

#include <Wire.h>

#define MPL3115A2_ADDRESS 0x60 // Unshifted 7-bit I2C address for sensor

#define MPL3115A2_STATUS     0x00
#define MPL3115A2_OUT_P_MSB  0x01
#define MPL3115A2_OUT_P_CSB  0x02
#define MPL3115A2_OUT_P_LSB  0x03
#define MPL3115A2_OUT_T_MSB  0x04
#define MPL3115A2_OUT_T_LSB  0x05
#define MPL3115A2_DR_STATUS  0x06
#define MPL3115A2_OUT_P_DELTA_MSB  0x07
#define MPL3115A2_OUT_P_DELTA_CSB  0x08
#define MPL3115A2_OUT_P_DELTA_LSB  0x09
#define MPL3115A2_OUT_T_DELTA_MSB  0x0A
#define MPL3115A2_OUT_T_DELTA_LSB  0x0B
#define MPL3115A2_WHO_AM_I   0x0C
#define MPL3115A2_F_STATUS   0x0D
#define MPL3115A2_F_DATA     0x0E
#define MPL3115A2_F_SETUP    0x0F
#define MPL3115A2_TIME_DLY   0x10
#define MPL3115A2_SYSMOD     0x11
#define MPL3115A2_INT_SOURCE 0x12
#define MPL3115A2_PT_DATA_CFG 0x13
#define MPL3115A2_BAR_IN_MSB 0x14
#define MPL3115A2_BAR_IN_LSB 0x15
#define MPL3115A2_P_TGT_MSB  0x16
#define MPL3115A2_P_TGT_LSB  0x17
#define MPL3115A2_T_TGT      0x18
#define MPL3115A2_P_WND_MSB  0x19
#define MPL3115A2_P_WND_LSB  0x1A
#define MPL3115A2_T_WND      0x1B
#define MPL3115A2_P_MIN_MSB  0x1C
#define MPL3115A2_P_MIN_CSB  0x1D
#define MPL3115A2_P_MIN_LSB  0x1E
#define MPL3115A2_T_MIN_MSB  0x1F
#define MPL3115A2_T_MIN_LSB  0x20
#define MPL3115A2_P_MAX_MSB  0x21
#define MPL3115A2_P_MAX_CSB  0x22
#define MPL3115A2_P_MAX_LSB  0x23
#define MPL3115A2_T_MAX_MSB  0x24
#define MPL3115A2_T_MAX_LSB  0x25
#define MPL3115A2_CTRL_REG1  0x26
#define MPL3115A2_CTRL_REG2  0x27
#define MPL3115A2_CTRL_REG3  0x28
#define MPL3115A2_CTRL_REG4  0x29
#define MPL3115A2_CTRL_REG5  0x2A
#define MPL3115A2_OFF_P      0x2B
#define MPL3115A2_OFF_T      0x2C
#define MPL3115A2_OFF_H      0x2D

class MPL3115A2 {

public:
  MPL3115A2();

  //Public Functions
  void begin(); // Gets sensor on the I2C bus.
  float readAltitude(); // Returns float with meters above sealevel. Ex: 1638.94
  float readAltitudeFt(); // Returns float with feet above sealevel. Ex: 5376.68
  float readPressure(); // Returns float with barometric pressure in Pa. Ex: 83351.25
  float readTemp(); // Returns float with current temperature in Celsius. Ex: 23.37
  float readTempF(); // Returns float with current temperature in Fahrenheit. Ex: 73.96
  void setModeBarometer(); // Puts the sensor into Pascal measurement mode.
  void setModeAltimeter(); // Puts the sensor into altimetery mode.
  void setModeStandby(); // Puts the sensor into Standby mode. Required when changing CTRL1 register.
  void setModeActive(); // Start taking measurements!
  void setOversampleRate(byte); // Sets the # of samples from 1 to 128. See datasheet.
  void enableEventFlags(); // Sets the fundamental event flags. Required during setup.

  //Public Variables

private:
  //Private Functions

  void toggleOneShot();
  byte IIC_Read(byte regAddr);
  void IIC_Write(byte regAddr, byte value);

  //Private Variables

};

#endif // End include guard
