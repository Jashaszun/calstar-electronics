#include <RFM69_ATC.h>     //get it here: https://www.github.com/lowpowerlab/rfm69
#include <SPI.h>           //included with Arduino IDE install (www.arduino.cc)
#include <Wire.h>
#include "LIS331HH.h"
#include <SparkFunMPL3115A2.h>

//*********************************************************************************************
//************ IMPORTANT SETTINGS - YOU MUST CHANGE/CONFIGURE TO FIT YOUR HARDWARE ************
//*********************************************************************************************
#define NODEID        (4)    // must be unique for each node on same network (range up to 254, 255 is used for broadcast)
#define NETWORKID     (100)  // the same on all nodes that talk to each other (range up to 255)
#define TRANSMIT_TO   (255)  // the node to send to (255 = broadcast)
//Match frequency to the hardware version of the radio on your Moteino (uncomment one):
#define FREQUENCY   RF69_433MHZ
//#define FREQUENCY   RF69_868MHZ
//#define FREQUENCY     RF69_915MHZ
#define ENCRYPTKEY    "sampleEncryptKey" //exactly the same 16 characters/bytes on all nodes!
#define IS_RFM69HW_HCW  //uncomment only for RFM69HW/HCW! Leave out if you have RFM69W/CW!
//*********************************************************************************************
//Auto Transmission Control - dials down transmit power to save battery
//Usually you do not need to always transmit at max output power
//By reducing TX power even a little you save a significant amount of battery power
//This setting enables this gateway to work with remote nodes that have ATC enabled to
//dial their power down to only the required level (ATC_RSSI)
#define ENABLE_ATC    //comment out this line to disable AUTO TRANSMISSION CONTROL
#define ATC_RSSI      -80
//*********************************************************************************************

#ifdef ENABLE_ATC
  RFM69_ATC radio;
#else
  RFM69 radio;
#endif

#include <Servo.h>
Servo scissorLiftServo;
MPL3115A2 altimeter;
LIS331HH accelerometer(0); // tie SAO to ground

int main(void) {
	init();

	pinMode(A1, OUTPUT);
	pinMode(A2, OUTPUT);
	pinMode(A3, OUTPUT);

	for (int i = 0; i < 3; i++) {
		digitalWrite(A2, HIGH);
		delay(500);
		digitalWrite(A2, LOW);
		delay(500);
		digitalWrite(A3, HIGH);
		delay(500);
		digitalWrite(A3, LOW);
		delay(500);
	}

	scissorLiftServo.attach(6);
	scissorLiftServo.write(90); // don't move

	Serial.begin(19200);
    Serial.println("Starting radio...");
    pinMode(A0, OUTPUT);
    digitalWrite(A0, HIGH);
    delay(500);
    digitalWrite(A0, LOW);
    delay(500);
    //Serial.println("Radio reset complete.");
    //Serial.println();

	radio.initialize(FREQUENCY,NODEID,NETWORKID);
	#ifdef IS_RFM69HW_HCW
  	radio.setHighPower(); //must include this only for RFM69HW/HCW!
	#endif
  	radio.encrypt(ENCRYPTKEY);

  	#ifdef ENABLE_ATC
  	radio.enableAutoPower(ATC_RSSI);
	#endif

  	//Serial.println("Receiving at 433 MHz...");

	#ifdef ENABLE_ATC
	//Serial.println("RFM69_ATC Enabled (Auto Transmission Control)\n");
	#endif

	pinMode(8, INPUT_PULLUP); // error flag from current loop
	pinMode(9, OUTPUT); // deployment voltage signal
	digitalWrite(9, LOW);
	pinMode(7, OUTPUT); // deployment current loop output disable
	pinMode(7, LOW);

	bool deploymentSignalOut = false;

	altimeter.begin();
    altimeter.setModeAltimeter();
    altimeter.setOversampleRate(7);
    altimeter.enableEventFlags();

	// Normal power mode, 1000 Hz data rate, x y and z axes enabled
    accelerometer.write_reg(LIS331HH_CTRL_REG1, 0x3F);
    // Block data update, +- 12g scale
    accelerometer.write_reg(LIS331HH_CTRL_REG4, 0x80);

	// for reading 3-digit integer from UART
	char input[3];
	int inputIndex = 0;

	char outbuf[100];

	byte count = 0;
	
	while (true) {
		if (count%16 == 0) {
			sprintf(outbuf, "Ac: x: %+07i y: %+07i z: %+07i  Al: %+07i\n", (int)(1000*accelerometer.get_x_g()), (int)(1000*accelerometer.get_y_g()), (int)(1000*accelerometer.get_z_g()), (int)(1000*altimeter.readAltitudeFt()));
			radio.sendWithRetry(TRANSMIT_TO, outbuf, strlen(outbuf));
			count = 0;
		}
		if (radio.receiveDone()) {
			for (byte i = 0; i < radio.DATALEN; i++) {
				Serial.print((char)radio.DATA[i]);
				if (radio.DATA[i] == 's') {
					deploymentSignalOut ^= true;
					inputIndex = 0;
				} else if (radio.DATA[i] >= '0' && radio.DATA[i] <= '9') {
					input[inputIndex] = radio.DATA[i];
					if (++inputIndex >= 3) {
						int value = 100*(input[0]-'0') + 10*(input[1]-'0') + (input[2]-'0');
						if (value >= 0 && value <= 180) {
							Serial.print("Setting servo to ");
							Serial.println(value);
							scissorLiftServo.write(value);
						}
						inputIndex = 0;
					}
				} else if (radio.DATA[i] == '\n') {
					inputIndex = 0;
				}
			}

			if (radio.ACKRequested()) {
				radio.sendACK();
			}


		}

		//digitalWrite(A3, red ? HIGH : LOW);
		//digitalWrite(A2, green ? HIGH : LOW);
		//digitalWrite(A1, blue ? HIGH : LOW);
		digitalWrite(9, deploymentSignalOut);
		digitalWrite(7, deploymentSignalOut);
		digitalWrite(A3, deploymentSignalOut); // red = sending signal
		digitalWrite(A2, !digitalRead(8)); // green = error flag from current loop



		++count;
	}
  	return 0;
}