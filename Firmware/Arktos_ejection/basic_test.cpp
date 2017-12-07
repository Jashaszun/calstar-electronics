#include <RFM69.h>     //get it here: https://www.github.com/lowpowerlab/rfm69
#include <SPI.h>           //included with Arduino IDE install (www.arduino.cc)
#include <Wire.h>
#include "LIS331HH.h"
#include <SparkFunMPL3115A2.h>
#include <Servo.h>

//*********************************************************************************************
//************ IMPORTANT SETTINGS - YOU MUST CHANGE/CONFIGURE TO FIT YOUR HARDWARE ************
//*********************************************************************************************
#define NODEID        (4)    // must be unique for each node on same network (range up to 254, 255 is used for broadcast)
#define NETWORKID     (100)  // the same on all nodes that talk to each other (range up to 255)
#define TRANSMIT_TO   (255)  // the node to send to (255 = broadcast)
#define FREQUENCY   RF69_433MHZ
#define ENCRYPTKEY    "sampleEncryptKey" //exactly the same 16 characters/bytes on all nodes!
//*********************************************************************************************
//Auto Transmission Control - dials down transmit power to save battery
//Usually you do not need to always transmit at max output power
//By reducing TX power even a little you save a significant amount of battery power
//This setting enables this gateway to work with remote nodes that have ATC enabled to
//dial their power down to only the required level (ATC_RSSI)
//#define ENABLE_ATC    //comment out this line to disable AUTO TRANSMISSION CONTROL
//#define ATC_RSSI      -80
//*********************************************************************************************

#ifdef ENABLE_ATC
  RFM69_ATC radio;
#else
  RFM69 radio;
#endif

Servo scissorLiftServo;
MPL3115A2 altimeter;
LIS331HH accelerometer(0); // tie SAO to ground

#define RADIO_RESET_PIN A0

#define GREEN_LED_PIN A2
#define RED_LED_PIN A3
#define BLUE_LED_PIN A1

#define SERVO_1_PIN 6

#define DEPLOYMENT_SIGNAL_PIN 9
#define XTR_EF_PIN 8
#define XTR_OD_PIN 7

void initPins(void) {
	pinMode(RED_LED_PIN, OUTPUT);
	pinMode(GREEN_LED_PIN, OUTPUT);
	pinMode(BLUE_LED_PIN, OUTPUT);

    pinMode(RADIO_RESET_PIN, OUTPUT);

	pinMode(DEPLOYMENT_SIGNAL_PIN, OUTPUT); // deployment voltage signal
	digitalWrite(DEPLOYMENT_SIGNAL_PIN, false);

	pinMode(XTR_EF_PIN, INPUT_PULLUP); // error flag from current loop
	pinMode(XTR_OD_PIN, OUTPUT); // deployment current loop output disable
	digitalWrite(XTR_OD_PIN, true);

	scissorLiftServo.attach(SERVO_1_PIN);
	scissorLiftServo.write(90); // don't move
}

void startRadio(void) {
    Serial.print("Starting radio...");
    digitalWrite(RADIO_RESET_PIN, true);
    digitalWrite(GREEN_LED_PIN, true);
    delay(500);
    digitalWrite(RADIO_RESET_PIN, false);
    digitalWrite(GREEN_LED_PIN, false);
    delay(500);
    Serial.println(" reset complete.");

	radio.initialize(FREQUENCY,NODEID,NETWORKID);
  	radio.setHighPower(); //must include this only for RFM69HW/HCW!
  	radio.encrypt(ENCRYPTKEY);

  	#ifdef ENABLE_ATC
  	radio.enableAutoPower(ATC_RSSI);
	#endif
}

void startSensors(void) {
	altimeter.begin();
    altimeter.setModeAltimeter();
    altimeter.setOversampleRate(7);
    altimeter.enableEventFlags();

	// Normal power mode, 1000 Hz data rate, x y and z axes enabled
    accelerometer.write_reg(LIS331HH_CTRL_REG1, 0x3F);
    // Block data update, +- 12g scale
    accelerometer.write_reg(LIS331HH_CTRL_REG4, 0x90);
}

void setDeplSignal(bool output) {
	digitalWrite(DEPLOYMENT_SIGNAL_PIN, output);
	digitalWrite(XTR_OD_PIN, !output);
	digitalWrite(RED_LED_PIN, output);
}

int main(void) {
	init();

	initPins();

	Serial.begin(19200);

	startRadio();

	startSensors();

	bool outputSensorData = true;

	float altMovingAvg = 0;
	float currentAltZero = 0;

	bool runningDeplSignal = false;
	unsigned long deplSignalStartTime = 0;

	bool runningServo = false;
	unsigned long servoStartTime = 0;

	unsigned int transmitInterval = 200; // in ms
	unsigned long lastTransmitTime = 0;

	// for reading 3-digit integer from UART/radio
	char input[3];
	int inputIndex = 0;
	while (true) {
		if (Serial.available() > 0) {
			char ch = Serial.read();
			if (ch == 'u') {
				transmitInterval += 50;
				Serial.print("Tx Interval = ");
				Serial.println(transmitInterval);
			} else if (ch == 'd') {
				transmitInterval -= 50;
				Serial.print("Tx Interval = ");
				Serial.println(transmitInterval);
			}
		}
		if (millis() - lastTransmitTime >= transmitInterval) {
			float accelX = accelerometer.get_x_g();
			float accelY = accelerometer.get_y_g();
			float accelZ = accelerometer.get_z_g();
			float alt = altimeter.readAltitudeFt();

			char outbuf[100];
			sprintf(outbuf, "Ac: x: %+07i y: %+07i z: %+07i  Al: %+07i\n",
				(int)(1000*accelX), (int)(1000*accelY), (int)(1000*accelZ), (int)(alt-currentAltZero));

			if (outputSensorData) {
				radio.send(TRANSMIT_TO, outbuf, strlen(outbuf));
				//Serial.println(outbuf);
			}
			#define ALPHA (.75f)
			altMovingAvg = ALPHA*altMovingAvg + (1-ALPHA)*alt;

			lastTransmitTime = millis();
		}

		if (radio.receiveDone()) {
			Serial.println("Received data!");
			if (radio.ACKRequested()) {
				radio.sendACK();
			}

			for (byte i = 0; i < radio.DATALEN; i++) {
				Serial.print((char)radio.DATA[i]);
				if (radio.DATA[i] == 'a' && !runningDeplSignal) { // activate
					setDeplSignal(true);
					inputIndex = 0;
					radio.sendWithRetry(TRANSMIT_TO, "S_ON", strlen("S_ON"), 5, 200);
				} else if (radio.DATA[i] == 'o' && !runningDeplSignal) { // off
					setDeplSignal(false);
					inputIndex = 0;
					radio.sendWithRetry(TRANSMIT_TO, "S_OFF", strlen("S_OFF"), 5, 200);
				} else if (radio.DATA[i] >= '0' && radio.DATA[i] <= '9') { // servo data
					input[inputIndex] = radio.DATA[i];
					if (++inputIndex >= 3) {
						int value = 100*(input[0]-'0') + 10*(input[1]-'0') + (input[2]-'0');
						static char message[] = "SERVOxxx";
						message[5] = input[0];
						message[6] = input[1];
						message[7] = input[2];
						if (value >= 0 && value <= 180) {
							// be safe with ejection servos -- if setting to no movement, do it quickly
							if (value == 90) {
								scissorLiftServo.write(value);
								radio.sendWithRetry(TRANSMIT_TO, message, strlen(message), 5, 200);
							} else {
								radio.sendWithRetry(TRANSMIT_TO, message, strlen(message), 5, 200);
								scissorLiftServo.write(value);
							}
						}
						inputIndex = 0;
					}
				} else if (radio.DATA[i] == 's') { // sensors on
					outputSensorData = true;
				} else if (radio.DATA[i] == 'x') { // sensors cut
					outputSensorData = false;
				} else if (radio.DATA[i] == 'z') { // zero altitude
					currentAltZero = altMovingAvg;
					radio.sendWithRetry(TRANSMIT_TO, "ALT_ZERO", strlen("ALT_ZERO"), 5, 200);
				} else if (radio.DATA[i] == 'p' && !runningDeplSignal) { // pulse deployment signal
					radio.sendWithRetry(TRANSMIT_TO, "S_PULSE", strlen("S_PULSE"), 5, 200);
					runningDeplSignal = true;
					deplSignalStartTime = millis();
					setDeplSignal(true);
				} else if (radio.DATA[i] == 'e' && !runningServo) { // eject
					radio.sendWithRetry(TRANSMIT_TO, "SERVO_PULSE", strlen("SERVO_PULSE"), 5, 200);
					runningServo = true;
					servoStartTime = millis();
					scissorLiftServo.write(88);
					digitalWrite(GREEN_LED_PIN, true);
				} else if (radio.DATA[i] == '\n') {
					inputIndex = 0;
				}
			}
		}
		if (runningDeplSignal) {
			if (millis() - deplSignalStartTime >= 2000) {
				setDeplSignal(false);
				runningDeplSignal = false;
				deplSignalStartTime = 0;
			}
		}
		if (runningServo) {
			if (millis() - servoStartTime >= 5000) {
				scissorLiftServo.write(90);
				digitalWrite(GREEN_LED_PIN, false);
				runningServo = false;
				servoStartTime = 0;
			}
		}
		// digitalWrite(GREEN_LED_PIN, !digitalRead(XTR_EF_PIN)); // green = error flag from current loop
	}
  	return 0;
}