#include <RFM69.h>         //get it here: https://www.github.com/lowpowerlab/rfm69
#include <RFM69_ATC.h>     //get it here: https://www.github.com/lowpowerlab/rfm69
#include <SPI.h>           //included with Arduino IDE install (www.arduino.cc)

//*********************************************************************************************
//************ IMPORTANT SETTINGS - YOU MUST CHANGE/CONFIGURE TO FIT YOUR HARDWARE ************
//*********************************************************************************************
#define NODEID        (2)    // must be unique for each node on same network (range up to 254, 255 is used for broadcast)
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

#define SERIAL_BAUD   19200

#ifdef ENABLE_ATC
  RFM69_ATC radio;
#else
  RFM69 radio;
#endif

int main(void) {
  init();

  Serial.begin(SERIAL_BAUD);
  //Serial.println("Resetting radio...");
  pinMode(9, OUTPUT);
  digitalWrite(9, HIGH);
  delay(500);
  digitalWrite(9, LOW);
  delay(500);
  //Serial.println("Radio reset complete.");
  //Serial.println();

  radio.initialize(FREQUENCY,NODEID,NETWORKID);
#ifdef IS_RFM69HW_HCW
  radio.setHighPower(); //must include this only for RFM69HW/HCW!
#endif
  radio.encrypt(ENCRYPTKEY);
  //radio.setFrequency(919000000); //set frequency to some custom frequency

//Auto Transmission Control - dials down transmit power to save battery (-100 is the noise floor, -90 is still pretty good)
//For indoor nodes that are pretty static and at pretty stable temperatures (like a MotionMote) -90dBm is quite safe
//For more variable nodes that can expect to move or experience larger temp drifts a lower margin like -70 to -80 would probably be better
//Always test your ATC mote in the edge cases in your own environment to ensure ATC will perform as you expect
#ifdef ENABLE_ATC
  radio.enableAutoPower(ATC_RSSI);
#endif

  //Serial.println("Transmitting at 433 MHz...");

#ifdef ENABLE_ATC
  //Serial.println("RFM69_ATC Enabled (Auto Transmission Control)\n");
#endif

  String line = "";
  while (true) {
    if (Serial.available() > 0) {
      char input = Serial.read();
      line += input;
      if (input == '\n') {
        radio.send(TRANSMIT_TO, line.c_str(), line.length());

        Serial.print("[SENDING] " + line);

        line = "";
      }
    }

    if (radio.receiveDone()) {
      if (radio.ACKRequested()) {
        radio.sendACK();
      }
    }
  }
  return 0;
}