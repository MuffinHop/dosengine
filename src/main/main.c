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
/*For testing...*/
float pyor=0.0f;
void demo() {
    word e;
    float i;
    word T = *my_clock;
    pyor = (float)(T)/60.0f;
    for(i=0.0; i<1.0f; i++){
        struct Vec z =  VecCreateF( 160.0f+i*4, 100.0f, 0.0f);
        struct Vec a = VecCreateF( 80.0f,-32.0f, 0.0f);
        struct Vec b = VecCreateF(-80.0f,-80.0f, 0.0f);
        struct Vec c = VecCreateF( i, 80.0f, 0.0f);
        a = rot2D( a, pyor+i/11.0f );
        b = rot2D( b, pyor+i/11.0f );
        c = rot2D( c, pyor+i/11.0f );
        a = add( a, z);
        b = add( b, z);
        c = add( c, z);
        polygonFillFixed(a.x, a.y, b.x, b.y, c.x, c.y);
    }
}
int main(int argc, char *argv[]) {
    union REGS regs;
    byte breakout = 0;
    int i,j,f,k=0;
    long oldCount = 0;
    int width = 320;
    int height = 200;
    int comp = 0;
    int req_comp = 0;
    int pixel_pos = 0;

    byte *image_pixelbuffer = malloc(320 * 200);
    byte *image_palettebuffer = malloc(256 * 3);


    
    calcSIN();
    calcCOS();


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
    startTime = 0;
    currentTime = 0;
    prevTime = 0;
    passedTime = 0;

    set_mode(VGA_256_COLOR_MODE);
    while ( !kbhit() ) {
        currentTime = frameCount;
        passedTime = frameCount - prevTime;
        prevTime = currentTime;

        displayImage(background);
        demo();
        wait_for_retrace();
        UpdateInfo();
    }

    midasMagicDies();
    set_mode(TEXT_MODE);
    return 0;
}

