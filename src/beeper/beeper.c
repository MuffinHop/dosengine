#include "src\misc\definer.h"

//Play sound using built in speaker
void play_sound(int nFrequence) {
        int Div;
        byte tmp;

        //Set the PIT to the desired frequency
        Div = 1193180 / nFrequence;
        outp(0x43, 0xb6);
        outp(0x42, (byte) (Div) );
        outp(0x42, (byte) (Div >> 8));
 
        //And play the sound using the PC speaker
        tmp = inp(0x61);
        if (tmp != (tmp | 3)) {
                outp(0x61, tmp | 3);
        }
}

//make it shutup
void nosound() {
        byte tmp = inp(0x61) & 0xFC;
        outp(0x61, tmp);
}
