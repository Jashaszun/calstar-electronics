// Access control: CalSTAR MR

#include <avr/io.h>
#include <util/delay.h>

#define F_CPU 16000000
#define ADC_THRESHOLD 512
#define ADC_IO DDRA
#define ADC_PORT PORTA
#define ADC_PIN 0
#define SOLENOID_IO DDRA
#define SOLENOID_PORT PORTA
#define SOLENOID_PIN 1
#define ACCEL_TOLERANCE 1
#define ALT_THRESHOLD
#define ACCEL_ADDR 0x00
#define ALT_ADDR 0x01

int main() {
	/* Pseudocode for deployment sequence:
	1. Wait until ADC input exceeds ADC threshold
	2. Verify vehicle is at rest on the ground with accelerometer/altimeter
	3. Actuate solenoid. */
}