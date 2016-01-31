#include "src\misc\definer.h"

dword *dwordVGA=(dword *)0xA0000; 

/*  pushes video buffer into vga card memory if under regular chunky mode;
    in unchained mode moves video display memory reading address*/
void push_videobuffer() {
    if(chunkymode != 1 && chainmode == 1) {
        word high_address,low_address;
        word temp = visible_page;
        visible_page = non_visible_page;
        non_visible_page = temp;
        high_address = HIGH_ADDRESS | (visible_page & 0xff00);
        low_address  = LOW_ADDRESS  | (visible_page << 8);
        outpw(CRTC_INDEX, high_address);
        outpw(CRTC_INDEX, low_address);
    } else {
        int i;
        dword *dwordVGA=(dword *)0xA0000; 
        dword *dwordBuffer=(dword *)&videobuffer;
        for(i = 0; i < 16000; i++) {
            dwordVGA[i] = dwordBuffer[i];
        }
    }
}
