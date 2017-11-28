#include <stdlib.h>
#include <stdio.h>
#include <avr/io.h>

/*
0) initialization:
    * turn current loop off (pull currlent loop high)   
1) Wait till radio transmission
2) verify landing
    * Ways to fail verification: still falling
    * if landing verification fails, wait for another radio signal
        - there should be a radio override to skip verification
3) if landing verified, start deployment
    * begin current loop transmission (pull low) -> should cause deployment, and wire should break
4) upon error flag (means separation has occurred) 
    a. wait some finite time 
    b. activate scissor lift
5) dont do anything more 
*/

/*
state machine:

initialization  --(upon initialization completion (no real event))-->           radio wait
radio wait      --(normal deploy signal)-->                                     verification
radio wait      --(force deploy signal)-->                                      deploy
verification    --(passes)-->                                                   deploy
verification    --(fails)-->                                                    radio wait
deploy          --(upon completion of activating deployment (no real event))--> error flag wait
error flag wait --(error flag recieved)-->                                      scissor lift
scissor lift    --(upon extending scissor lift (no real event)) -->             self disable (do literally nothing)
*/
#include <string.h>
#include <util/delay.h>
#include <Arduino.h>
#include <RFM69_ATC.h>
#include <SparkFunMPL3115A2.h>
#include <Wire.h>

#define CURRENTLOOP_OD 7 // output disable
#define CURRENTLOOP_EF 8 // error flag

#define ALT_THRESHOLD 50

#define RADIO_RESET 14
#define NODEID (3)
#define NETWORKID (100)
#define TRANSMIT_TO (255) // broadcast
#define FREQUENCY RF69_433MHZ
#define ENCRYPTKEY "sampleEncryptKey"
#define ATC_RSSI -80

// With current code, the length of the normal deploy signal, 
//  and the length of the force deploy signal MUST be the same
#define NORMAL_DEPLOY_SIGNAL "N DEPLOY" 
#define FORCE_DEPLOY_SIGNAL "F DEPLOY"
#define RADIO_SIGNAL_LEN 8

enum State {
    INIT,
    RADIO_WAIT,
    VERIFICATION,
    DEPLOY,
    ERROR_FLAG_WAIT,
    SCISSOR_LIFT_ACTIVATE,
    DISABLED
};

int main() {

    MPL3115A2 altimeter;
    RFM69_ATC radio;

    State state = INIT;

    while (true) {
        bool acceleration_flag = false;
        bool altimeter_flag = false;
        switch (state) {
            case INIT:
                init();
                Wire.begin();

                // Current loop initialization
                pinMode(CURRENTLOOP_OD, OUTPUT);
                digitalWrite(CURRENTLOOP_OD, HIGH);
                pinMode(CURRENTLOOP_EF, INPUT);

                altimeter.begin();
                altimeter.setModeAltimeter();
                altimeter.setOversampleRate(7);
                altimeter.enableEventFlags();

                radio.initialize(FREQUENCY, NODEID, NETWORKID);
                radio.setHighPower();
                radio.encrypt(ENCRYPTKEY);
                radio.enableAutoPower(ATC_RSSI);

                state = RADIO_WAIT;
                break;
            case RADIO_WAIT:

                if (radio.receiveDone()) {
                    char signal[RADIO_SIGNAL_LEN];

                    // if length of recieved data is not same as expected size ignore
                    if (radio.DATALEN != RADIO_SIGNAL_LEN)
                        break;

                    for (byte i = 0; i < radio.DATALEN; ++i)
                        signal[i] = (char)radio.DATA[i];
                    
                    if (strcmp(signal, NORMAL_DEPLOY_SIGNAL) == 0) {
                        state = VERIFICATION;
                    } else if (strcmp(signal, FORCE_DEPLOY_SIGNAL) == 0) {
                        state = DEPLOY;
                    }
                }

                break;
            case VERIFICATION:
                //check for acceleration (should be 0 or however we calibrate) 
                //  and for altitude (should be on ground)

                acceleration_flag = true; // actually implement accelerometer stuff
                    // and get accelerometer working.
                altimeter_flag = altimeter.readAltitudeFt() < ALT_THRESHOLD;

                if (acceleration_flag && altimeter_flag) {
                    state = DEPLOY;
                } else {
                    state = RADIO_WAIT;
                }

                break;
            case DEPLOY:
                // raise current
                digitalWrite(CURRENTLOOP_OD, LOW);

                state = ERROR_FLAG_WAIT;
                break;            
            case ERROR_FLAG_WAIT:
                if (digitalRead(CURRENTLOOP_EF) == LOW) {
                    // delay to make sure deployment is complete
                    for (int i = 0; i < 10; ++i)
                        _delay_ms(100);

                    state = SCISSOR_LIFT_ACTIVATE;
                }
                break;            
            case SCISSOR_LIFT_ACTIVATE:
                //should actually do servo stuff
                state = DISABLED;
                break;

            case DISABLED:
                // do literally nothing. maybe should disable stuff or something tho.
                break;
        }
    }

}
