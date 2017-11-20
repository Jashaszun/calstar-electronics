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
potential state machine (if doing that):

initialization  --(upon initialization completion (no real event))-->           radio wait
radio wait      --(normal radio signal)-->                                      verification
radio wait      --(force deployment radio signal)-->                            deploy
verification    --(verification succeeds)-->                                    deploy
verification    --(verification fails)-->                                       radio wait
deploy          --(upon completion of activating deployment (no real event))--> error flag wait
error flag wait --(error flag recieved)-->                                      scissor lift
scissor lift    --(upon extending scissor lift (no real event)) -->             self disable (do literally nothing)

*/


int main() {

}