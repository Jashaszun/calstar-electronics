#include <mbed.h>
#include "lib/MPU9250AHRS/MPU9250.h"
#include "lib/MPL3115A2/MPL3115A2.h"

// Pin definitions
#define I2C_SENSOR_SDA PB_7
#define I2C_SENSOR_SCL PB_6
#define TPC_RX PA_10
#define TPC_TX PA_9
#define EC_RX PC_7
#define EC_TX PC_6
#define DEBUG_RX PA_3
#define DEBUG_TX PA_2
#define STATE_LED_RED PA_8
#define STATE_LED_GREEN PC_9
#define STATE_LED_BLUE PC_8

// TODO define bp stuff

I2C i2c_sensors(I2C_SENSOR_SDA, I2C_SENSOR_SCL);
const int ALTIMETER_I2C_ADDR = 0xC0;
const int IMU_I2C_ADDR = 0xD0;

// TODO baud rate?
Serial tpc_talk(TPC_TX, TPC_RX);
Serial ec_talk(EC_TX, EC_RX);
Serial debug_talk(DEBUG_TX, DEBUG_RX);

DigitalOut led_red(STATE_LED_RED);
DigitalOut led_green(STATE_LED_GREEN);
DigitalOut led_blue(STATE_LED_BLUE);

int main() {
    return 0;
}
