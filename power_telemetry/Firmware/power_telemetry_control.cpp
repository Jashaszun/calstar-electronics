#include "mbed.h"
#include <RFM69.h>

#define NODEID        (4)    // must be unique for each node on same network (range up to 254, 255 is used for broadcast)
#define NETWORKID     (100)  // the same on all nodes that talk to each other (range up to 255)
#define TRANSMIT_TO   (255)  // the node to send to (255 = broadcast)
#define FREQUENCY   RF69_433MHZ
#define ENCRYPTKEY    "sampleEncryptKey" //exactly the same 16 characters/bytes on all nodes!

#define RADIO_RESET_PIN A0

#define GREEN_LED_PIN A2

RFM69 radio;

void startRadio(void) {
    /***
    Serial.print("Starting radio...");
    digitalWrite(GREEN_LED_PIN, true);
    delay(500);
    digitalWrite(RADIO_RESET_PIN, false);
    digitalWrite(GREEN_LED_PIN, false);
    delay(500);
    Serial.println(" reset complete.");
    **/

	radio.initialize(FREQUENCY,NODEID,NETWORKID);
  	radio.setHighPower(); //must include this only for RFM69HW/HCW!
  	radio.encrypt(ENCRYPTKEY);
}

int main(){
    startRadio();
    while(true){
        debug("printing");
    }

}
