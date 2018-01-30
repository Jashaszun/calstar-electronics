/*
Makefile:

PSRC = accelerometer_test.cpp

ARDLIBS = Wire LIS331HH

*/

#include <Arduino.h>
#include <LIS331HH.h>

int main() {
    init();

    Serial.begin(19200);
    Serial.println("Beginning accelerometer test program.");
    Serial.println("Should recieve messages in format \"Accel: x: num, y: num, z: num\".");
    Serial.println("Readings will be in g's.");

	Wire.begin();
    LIS331HH accelerometer(0);

    Serial.print("Accelerometer 7-bit address: ");
	Serial.println(accelerometer.get_i2c_addr());

	// Normal power mode, 1000 Hz data rate, x y and z axes enabled
	accelerometer.write_reg(LIS331HH_CTRL_REG1, 0x3F);

	// Block data update, +- 12g scale
	accelerometer.write_reg(LIS331HH_CTRL_REG4, 0x90);

	Serial.print("CTRL_REG1: ");
	Serial.println(accelerometer.read_reg(LIS331HH_CTRL_REG1));

	Serial.print("CTRL_REG4: ");
	Serial.println(accelerometer.read_reg(LIS331HH_CTRL_REG4));

	Serial.print("Scale (expect whatever it was set to in CTRL_REG4): ");
	Serial.println(accelerometer.get_scale());

	while (true) {
		Serial.print("Accel: x: ");
		Serial.print(accelerometer.get_x_g());
		
		Serial.print("    y: ");
		Serial.print(accelerometer.get_y_g());

		Serial.print("    z: ");
		Serial.println(accelerometer.get_z_g());
	}
}