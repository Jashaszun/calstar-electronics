#include <mbed.h>
#include "lib/MPU9250AHRS/MPU9250.h"
#include "lib/MPL3115A2/MPL3115A2.h"

#define BLACKBOX

// Pin definitions - Flight computers
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
#define BP1_IGNITE PB_15
#define BP1_CONT   PB_14
#define BP2_IGNITE PB_13
#define BP2_CONT   PB_12
#define BP3_IGNITE PB_10
#define BP3_CONT   PB_2
#define BP4_IGNITE PB_0
#define BP4_CONT   PB_1
#define BP5_IGNITE PC_5
#define BP5_CONT   PC_4
#define BP6_IGNITE PA_7
#define BP6_CONT   PA_6
#define BP7_IGNITE PA_5
#define BP7_CONT   PA_4

#ifdef BLACKBOX
// Pin definitions - Blackbox (for testing)
#define I2C_SENSOR_SDA PB_7
#define I2C_SENSOR_SCL PB_6
#define STATE_LED_RED PC_8
#define STATE_LED_GREEN PA_8
#define STATE_LED_BLUE PC_9
#endif

#define BAUDRATE 9600

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

Altitude altitude;
MPL3115A2 altimeter(&i2c_sensors, &debug_talk);

MPU9250 imu;

void led_color(bool r, bool g, bool b) {
    // Flip because common anode
    led_red.write(!r);
    led_green.write(!g);
    led_blue.write(!b);
}

void bp_test() {

    DigitalOut bp7_ig(BP7_IGNITE);
    bp7_ig = 0;

    while (true) {
        bp7_ig = 1;
        wait(0.5);
        bp7_ig = 0;
        wait(0.5);
    }
}

void rainbow_test() {
    while (true) {
        for (int r = 0; r < 2; r++) {
            for (int g = 0; g < 2; g++) {
                for (int b = 0; b < 2; b++) {
                    led_color(r, g, b);
                    wait(1);
                }
            }
        }
    }
}

void altimeter_test() {
    led_color(0, 0, 1);
    altimeter.init();
    debug_talk.baud(BAUDRATE);
    debug_talk.printf("\nBegin altimeter test\n");
    uint8_t whoami = altimeter.whoAmI();
    debug_talk.printf("Whoami: 0x%x\n", whoami);
    if (whoami == 0xC4) {
        debug_talk.printf("Successfully connected\n");
        Altitude a;
        Temperature t;
        Pressure p;
        altimeter.setOffsetAltitude(83);
        altimeter.setOffsetTemperature(20);
        altimeter.setOffsetPressure(-32);
        while (true) {


        altimeter.readAltitude(&a);
        altimeter.readTemperature(&t);
        
        altimeter.setModeStandby();
        altimeter.setModeBarometer();
        altimeter.setModeActive();
        altimeter.readPressure(&p);
        
        debug_talk.printf("Altitude: %sft, Temp: %sºF, Pressure: %sPa\r\n", a.print(), t.print(), p.print());
        debug_talk.printf("OFF_H: 0x%X, OFF_T: 0x%X, OFF_P: 0x%X\r\n", altimeter.offsetAltitude(), altimeter.offsetTemperature(), altimeter.offsetPressure());
    
        altimeter.setModeStandby();
        altimeter.setModeAltimeter();
        altimeter.setModeActive();
        }
    } else {
        debug_talk.printf("Failed to connect to altimeter\n");
    }
}

void imu_test() {
    // Could just use:
    // https://os.mbed.com/users/onehorse/code/MPU9250AHRS/file/4e59a37182df/main.cpp/
    led_color(0, 1, 0);
    debug_talk.baud(BAUDRATE);
    debug_talk.printf("\nBegin imu test\n");
    uint8_t whoami = imu.readByte(MPU9250_ADDRESS, WHO_AM_I_MPU9250);
    debug_talk.printf("Whoami: 0x%x\n", whoami);
    if (whoami == 0x71) {
        debug_talk.printf("Successfully connected\n");
    } else {
        debug_talk.printf("Failed to connect to IMU\n");
    }
}

int main() {
    //altimeter_test();
    imu_test();
}