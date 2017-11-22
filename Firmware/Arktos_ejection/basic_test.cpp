#include <RFM69.h>         //get it here: https://www.github.com/lowpowerlab/rfm69
#include <RFM69_ATC.h>     //get it here: https://www.github.com/lowpowerlab/rfm69
#include <SPI.h>           //included with Arduino IDE install (www.arduino.cc)

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

int main(void) {
	init();

	Serial.begin(19200);
    //Serial.println("Resetting radio...");
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

	pinMode(A1, OUTPUT);
	pinMode(A2, OUTPUT);
	pinMode(A3, OUTPUT);

	bool red = false,
		 green = false,
		 blue = false;

	while (true) {
		if (radio.receiveDone()) {
			Serial.print("[RECEIVED]: ");
			for (byte i = 0; i < radio.DATALEN; i++) {
				if (radio.DATA[i] == 'r')
					red ^= true;
				else if (radio.DATA[i] == 'g')
					green ^= true;
				else if (radio.DATA[i] == 'b')
					blue ^= true;
				else if (radio.DATA[i] == 'c') {
					red = green = blue = false;
				}
				else if (radio.DATA[i] == 'a') {
					red = green = blue = true;
				}
				if (radio.DATA[i] == '\n')
					Serial.println();
				else Serial.print((char)radio.DATA[i]);
			}

			if (radio.ACKRequested()) {
				radio.sendACK();
			}


			Serial.print("RGB=");
			Serial.print(red ? '1' : '0');
			Serial.print(green ? '1' : '0');
			Serial.println(blue ? '1' : '0');
		}

		digitalWrite(A3, red ? HIGH : LOW);
		digitalWrite(A2, green ? HIGH : LOW);
		digitalWrite(A1, blue ? HIGH : LOW);
	}
  	return 0;
}