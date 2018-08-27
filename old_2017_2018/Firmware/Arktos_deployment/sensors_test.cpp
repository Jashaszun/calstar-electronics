// Access control: CalSTAR MR

#include <Arduino.h>
#include <Wire.h>
#include <SparkFunMPL3115A2.h>
#include <LIS331HH.h>

#define F_CPU 16000000
#define BAUD_RATE 19200

int main() {
  // Setup
  init(); // always include this when using Arduino.h!
  Serial.begin(BAUD_RATE);
  Serial.print("Serial interface initialized with a baud rate of ");
  Serial.println((int)BAUD_RATE);
  MPL3115A2 altimeter;
  LIS331HH accelerometer(0); // tie SA0 to ground

  // Altimeter setup
  altimeter.begin();
  altimeter.setModeAltimeter();
  altimeter.setOversampleRate(7);
  altimeter.enableEventFlags();

  // Accelerometer setup
  accelerometer.write_reg(LIS331HH_CTRL_REG1, 0x3F); // Normal power mode, 1000 Hz data rate, x y and z axes enabled
  accelerometer.write_reg(LIS331HH_CTRL_REG4, 0x90); // Block data update, +- 12g scale

  while (true) {
    float accelX = accelerometer.get_x_g();
    float accelY = accelerometer.get_y_g();
    float accelZ = accelerometer.get_z_g();
    float alt = altimeter.readAltitudeFt();

    char outbuf[100];
    sprintf(outbuf, "Ac: x: %+07i y: %+07i z: %+07i  Al: %+07i\n", (int)(1000*accelX), (int)(1000*accelY), (int)(1000*accelZ), (int)(alt));
    Serial.println(outbuf);
  }
}
