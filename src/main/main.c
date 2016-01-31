#include "src\misc\definer.h"
void MIDAS_CALL UpdateInfo(void) {   
    static MIDASplayStatus status;

    /* Get playback status: */
    if ( !MIDASgetPlayStatus(playHandle, &status) )
        MIDASerror();

    /* Store interesting information in easy-to-access variables: */
    position = status.position;
    pattern = status.pattern;
    row = status.row;
    syncInfo = status.syncInfo;
}
int main(int argc, char *argv[]) {
    union REGS regs;
    int i,j,f,k=0;
    long oldCount = 0;
    int width = 320;
    int height = 200;
    int comp = 0;
    int req_comp = 0;
    int pixel_pos = 0;

    byte *image_pixelbuffer = malloc(320 * 200);
    byte *image_palettebuffer = malloc(256 * 3);

    /* Unbuffer stdout: */
    setbuf(stdout, NULL);

    /* Prevent warnings: */
    argc = argc;
    argv = argv;

    start = *my_clock;

    system("cls");
    printf("Please wait...");

    loader();

    /* Play the tune */
    if ( (playHandle = MIDASplayModule(module, TRUE)) == 0 ) {
        MIDASerror();
    }

            
    while ( !kbhit() ) {
        if(row<32){
            displayImage(background, 0);
        }else if(row>32){
            
            for(j=0; j<32; j++) {
            for(i=0; i<32; i++) {
                extremeBuffer[i+j*240]=63;
            }}
            for(j=20; j<52; j++) {
            for(i=40; i<72; i+=3) {
                extremeBuffer[i+j*240]=63;
            }}
            for(j=10; j<42; j++) {
            for(i=32; i<62; i+=3) {
                extremeBuffer[i+j*240]=63;
            }}

            regs.h.ah = 0x10;
            regs.h.al = 0x01;
            regs.h.bh = i&255;
            int386(0x10,&regs,&regs);
            extremeRender();
        }
        if(row==32)extremeMode();
        if(row==0)set_mode(VGA_256_COLOR_MODE);
        wait_for_retrace();
        UpdateInfo();
        vgaSetBorder(1);
    }
    getch();

    midasMagicDies();
    set_mode(TEXT_MODE);

    return 0;
}

