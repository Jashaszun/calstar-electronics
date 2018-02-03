// Access control: CalSTAR MR

#include <avr/io.h>
#include <util/delay.h>
#include <Arduino.h>
#include <Wire.h>
#include <SparkFunMPL3115A2.h>
#include <LIS331HH.h>
#include <RFM69.h>
#include <Servo.h>

#define F_CPU 16000000
#define RECEIVER_IO DDRD
#define RECEIVER_PORT PIND
#define RECEIVER_PIN 7
#define TRANSMITTER_IO DDRB
#define TRANSMITTER_PORT PORTB
#define TRANSMITTER_PIN 0
#define LED_IO DDRD
#define LED_PORT PORTD
#define LED_PIN_RED 5
#define LED_PIN_GREEN 7
#define LED_PIN_BLUE 6
#define ALT_LAND 50
#define ALT_LAUNCHED 200
#define BASE_ALT 0
#define VERIF_SAMPLES 5
#define SERVO_PIN 6
// From old code - update
#define RADIO_RESET_PIN A0
#define NODEID (4)
#define NETWORKID (100)
#define TRANSMIT_TO (255) // broadcast
#define FREQUENCY RF69_433MHZ
#define ENCRYPTKEY "sampleEncryptKey"

enum State {
  INIT,
  LAUNCH_PAD,
  LAUNCHED,
  RADIO_WAIT,
  DEPLOY,
  LVDS_WAIT,
  SCISSOR_LIFT_ACTIVATE,
  DISABLED
};

short launched(MPL3115A2 &altimeter);
short landed(MPL3115A2 &altimeter);
short deploymentDisconnect();

int main() {
  init(); // always call this first if using Arduino.h!
  Wire.begin(); // for I2C
  // Create peripherals
  MPL3115A2 altimeter;
#ifdef ENABLE_ATC
  RFM69_ATC radio;
#else
  RFM69 radio;
#endif
  Servo servo;
  LIS331HH accelerometer(0);

  // Set outputs and inputs; receiver defaults to input
  LED_IO = (1 << LED_PIN_RED) | (1 << LED_PIN_GREEN) | (1 << LED_PIN_BLUE); // set LEDs as outputs
  TRANSMITTER_IO = 0xFF; // set LVDS transmitter as output
  TRANSMITTER_PORT |= (1 << TRANSMITTER_PIN);

  pinMode(RADIO_RESET_PIN, OUTPUT);

  State state = INIT;

  while (true) {
    switch (state) {
      case INIT:
        // Initialize
        altimeter.begin();
        altimeter.setModeAltimeter();
        altimeter.setOversampleRate(7);
        altimeter.enableEventFlags();

        digitalWrite(RADIO_RESET_PIN, HIGH);
        delay(500);
        digitalWrite(RADIO_RESET_PIN, LOW);
        delay(500);

        radio.initialize(FREQUENCY, NODEID, NETWORKID);
        radio.setHighPower();
        radio.encrypt(ENCRYPTKEY);
        #ifdef ENABLE_ATC
        radio.enableAutoPower(ATC_RSSI)
        #endif

        servo.attach(SERVO_PIN);

        // CHANGE
        //  |
        //  |
        //  |
        //  |
        // \/
        servo.write(90); // NEEDS TO BE CHANGED <--------- CHANGE
        // ^
        // |
        // |
        // |
        // |
        // CHANGE


        accelerometer.write_reg(LIS331HH_CTRL_REG1, 0x3F); // Normal power mode, 1000 Hz data rate, x y and z axes enabled
        accelerometer.write_reg(LIS331HH_CTRL_REG4, 0x90); // Block data update, +- 12g scale

        state = LAUNCH_PAD;
        break;
      case LAUNCH_PAD:
        if (launched(altimeter)) {
          state = LAUNCHED;
        }
        break;
      case LAUNCHED:
        if (landed(altimeter)) {
          state = RADIO_WAIT;
        }
        break;
      case RADIO_WAIT:
        // ARE WE WAITING FOR A DEPLOYMENT SIGNAL?????

        // if (radio.receiveDone()) {
        //   if (radio.ACKRequested()) {
        //     radio.sendACK();
        //   }
        //   char signal[RADIO_SIGNAL_LEN];
        //   // if length of recieved data is not same as expected size ignore
        //   if (radio.DATALEN != RADIO_SIGNAL_LEN)
        //     break;

        //   for (byte i = 0; i < radio.DATALEN; ++i)
        //     signal[i] = (char)radio.DATA[i];

        //   if (strcmp(signal, NORMAL_DEPLOY_SIGNAL) == 0) {
        //     state = VERIFICATION;
        //   } else if (strcmp(signal, FORCE_DEPLOY_SIGNAL) == 0) {
        //     state = DEPLOY;
        //   }
        // }
        break;
      case DEPLOY:
        TRANSMITTER_PORT &= ~(1 << TRANSMITTER_PIN); // send deployment signal
        state = LVDS_WAIT;
        break;
      case LVDS_WAIT:
        if (deploymentDisconnect()) {
          state = SCISSOR_LIFT_ACTIVATE;
        }
        break;
      case SCISSOR_LIFT_ACTIVATE:
        // NEED TO WRITE
        // NEED TO WRITE
        // NEED TO WRITE
        // NEED TO WRITE
        // NEED TO WRITE
        // NEED TO WRITE
        // NEED TO WRITE
        state = DISABLED;
        break;
      case DISABLED:
        break;
    }
  }
}

short launched(MPL3115A2 &altimeter) {
	static int count = 0;
	if (altimeter.readAltitudeFt() - BASE_ALT > ALT_LAUNCHED) {
		count++;
		if (count >= VERIF_SAMPLES) { // need five signals to verify
			return 1; // yay we launched
		}
	} else {
		count = 0;
	}
	return 0;
}

short landed(MPL3115A2 &altimeter) {
	for (int i = 0; i < VERIF_SAMPLES; i++) {
		if (altimeter.readAltitudeFt() - BASE_ALT > ALT_LAND) {
			return 0; // if any of the first five signals disagree, return - resend radio signal to retrigger
		}
	}
	return 1;
}

short deploymentDisconnect() {
  for (int i = 0; i < VERIF_SAMPLES; i++) {
    if (!(RECEIVER_PORT & (1 << RECEIVER_PIN))) {
      return 0;
    }
    delay(10);
  }
  return 1;
}
