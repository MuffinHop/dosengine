#include "src\misc\definer.h"

dword *dwordVGA=(dword *)0xA0000; 

/*pushes video buffer into vga card memory if under regular chunky mode*/
void push_videobuffer() {
    int i;
    dword *dwordVGA=(dword *)0xA0000; 
    dword *dwordBuffer=(dword *)&videobuffer[0];
    if(chunkymode != 1) {
        set_mode(TEXT_MODE);
        printf("Don't use push_buffer in other modes.");
        return;
    }
    for(i = 0; i < 16000; i++) {
        dwordVGA[i] = dwordBuffer[i];
    }
}
