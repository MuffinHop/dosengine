#include "src\misc\definer.h"
void set_mode(byte mode) {
  union REGS regs;

  regs.h.ah = SET_MODE;
  regs.h.al = mode;
  int386(VIDEO_INT, &regs, &regs);
}
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
void demo() {
    int i = 0;
    float x, y;
    for( y = 0; y < 200; y++) {
        for( x = 0; x < 320; x++) {
            float cf = (x + y)/(320.0f*200.0f);
            int ci;
            cf *= 5120.0f;
            ci = (int)cf;
            i++;
            videobuffer[0][i] = ci;
        }
    }
    wait_for_retrace();
}
int main(int argc, char *argv[]) {
    union REGS regs;
    int i,f,k=0;
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

    for(i=0;i<256;i++){
        outp(0x03c8, (byte)i);
        outp(0x03c9, (byte)i);
        outp(0x03c9, (byte)i);
        outp(0x03c9, (byte)i);
    }
    if ( (playHandle = MIDASplayModule(module, TRUE)) == 0 )
        MIDASerror();
    while ( !kbhit() ) {
        displayImage(background, 0);
        wait_for_retrace();
        UpdateInfo();
        vgaSetBorder(1);
        waitFrame();
    }
    getch();

    midasMagicDies();
    set_mode(TEXT_MODE);

    return 0;
}

