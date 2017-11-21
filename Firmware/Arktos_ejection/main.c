#include <stdlib.h>
#include <stdio.h>
#include <avr/io.h>

#define F_CPU (16000000UL)

/*
0) initialization:
    * turn current loop off (pull currlent loop high)   
1) Wait till radio transmission (we are using interrupts?)
2) verify landing
    * Ways to fail verification: still falling
    * if landing verification fails, wait for another radio signal
        - should we have an override radio signal as well, in case landing verification fails but we know (visually) that we are on the ground?
            . this would skip verification and begin deployment
            . so 2 types of signals, 1 normal, 1 force
3) if landing verified, start deployment
    * begin current loop transmission (pull low) -> should cause deployment, and wire should break
4) upon error flag (means separation has occurred) (can we make this an interrupt as well? <- cuz then interrupts would work nicely as events to change to state in a state machine)
    a. wait some finite time
    b. activate scissor lift
5) dont do anything more 
*/

/*
state machine:

initialization  --(upon initialization completion (no real event))-->           radio wait
radio wait      --(radio signal)-->                                             deploy
deploy          --(upon completion of activating deployment (no real event))--> error flag wait
error flag wait --(error flag recieved)-->                                      scissor lift
scissor lift    --(upon extending scissor lift (no real event)) -->             self disable (do literally nothing)
*/

#include <RFM69_ATC.h>
#include <avr/io.h>
#include <string.h>

#define CURRENTLOOP_OD 7 // output disable
#define CURRENTLOOP_EF 8 // error flag

#define RADIO_RESET 14
#define NODEID (3)
#define NETWORKID (100)
#define TRANSMIT_TO (255) // broadcast
#define FREQUENCY RF69_433MHZ
#define ENCRYPTKEY "sampleEncryptKey"
#define ATC_RSSI -80

#define RADIO_SIGNAL "START"
#define RADIO_SIGNAL_LEN 5

typedef enum {
    INIT,
    RADIO_WAIT,
    DEPLOY,
    ERROR_FLAG_WAIT,
    SCISSOR_LIFT_ACTIVATE,
    PARTY
} State;

int main() {

    RFM69_ATC radio;

    State state = INIT;

    while (true) {
        switch (state) {
            case INIT:
                // Current loop initialization
                pinMode(CURRENTLOOP_OD, OUTPUT);
                digitalWrite(CURRENTLOOP_OD, HIGH);

                pinMode(CURRENTLOOP_EF, INPUT);

                radio.initialize(FREQUENCY, NODEID, NETWORKID);
                radio.setHighPower();
                radio.encrypt(ENCRYPTKEY);
                radio.enableAutoPower(ATC_RSSI);

                state = RADIO_WAIT;
                break;
            case RADIO_WAIT:

                if (radio.recieveDone()) {
                    char signal[RADIO_SIGNAL_LEN];

                    // if length of recieved data is not same as expected size ignore
                    if (radio.DATALEN != RADIO_SIGNAL_LEN)
                        break;

                    for (byte i = 0; i < radio.DATALEN; ++i)
                        signal[i] = (char)radio.DATA[i];
                    
                    if (strcmp(signal, RADIO_SIGNAL) == 0)
                        state = DEPLOY;
                }

                break;       
            case DEPLOY:
                // raise current
                digitalWrite(CURRENTLOOP_OD, LOW);

                state = ERROR_FLAG_WAIT;
                break;            
            case ERROR_FLAG_WAIT:
                if (digitalRead(CURRENTLOOP_EF) == LOW)
                    state = SCISSOR_LIFT_ACTIVATE;
                break;            
            case SCISSOR_LIFT_ACTIVATE:

                state = PARTY;
                break;

            case PARTY:
                // do literally nothing/party. maybe should disable stuff or something tho.
                break;
        }
    }

}