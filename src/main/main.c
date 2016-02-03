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
    pyor = currentTime * 100;
    for(i=6.0; i>1.0f; i--){
        struct Vec a = VecCreateF( 110.0f,-122.0f, fmod(i+currentTime,12.0f)+1.2f);
        struct Vec b = VecCreateF(-110.0f,-110.0f, fmod(i+currentTime,12.0f)+1.2f);
        struct Vec c = VecCreateF( i, 110.0f, fmod(i,12.0f)+1.2f);
        struct Vec z =  VecCreateF( 160.0f+sin(currentTime+i*3.141591f*2.0f/6.0f)*64.0f, 100.0f+cos(currentTime+i*3.141591f*2.0f/6.0f)*64.0f, 0.0f);
        
        rot2D( &a, pyor + i * 32 );
        rot2D( &b, pyor + i * 32 );
        rot2D( &c, pyor + i * 32 );
        a.x = FIXEDDIV(a.x, a.z);
        a.y = FIXEDDIV(a.y, a.z);
        b.x = FIXEDDIV(b.x, b.z);
        b.y = FIXEDDIV(b.y, a.z);
        c.x = FIXEDDIV(c.x, a.z);
        c.y = FIXEDDIV(c.y, a.z);
        add( &a, &z);
        add( &b, &z);
        add( &c, &z);
        polygonFillFixed(a.x, a.y, b.x, b.y, c.x, c.y);
    }
    drawHorizontalLines();
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

    char str1[] = "hello, synctracker!\n";
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
    connectToCOM1();

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
        currentTime = frameCount / 60.0f;
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

