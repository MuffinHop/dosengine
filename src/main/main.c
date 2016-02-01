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
void line(int x0, int y0, int x1, int y1) {
  int dx = abs(x1-x0), sx = x0<x1 ? 1 : -1;
  int dy = abs(y1-y0), sy = y0<y1 ? 1 : -1; 
  int err = (dx>dy ? dx : -dy)/2, e2;
 
  while(1){
    videobuffer[x0+(y0*320)] = 44;
    if (x0==x1 && y0==y1) break;
    e2 = err;
    if (e2 >-dx) { err -= dy; x0 += sx; }
    if (e2 < dy) { err += dx; y0 += sy; }
  }
}
void filling(float x1, float y1, float x2, float y2, float x3, float y3) {
    float invslope1, invslope2, curx1, curx2;
    int scanlineY;
    invslope1 = (x2 - x1) / (y2 - y1);
    invslope2 = (x3 - x1) / (y3 - y1);

    curx1 = x1;
    curx2 = x1;
    scanlineY = y1;
    if(y1 != y2)
    for ( scanlineY = y1+1; scanlineY < y2; scanlineY++) {
        int x0 = curx1;
        int x1 = curx2;
        drawHorizontalLine( x0, x1, scanlineY, 33);
        curx1 += invslope1;
        curx2 += invslope2;
    }

    invslope1 = (x3 - curx1) / (y3 - scanlineY);
    invslope2 = (x3 - curx2) / (y3 - scanlineY);

    curx1 = curx1;
    curx2 = curx2;
    if(y2 != y3)
    for ( scanlineY = scanlineY; scanlineY < y3; scanlineY++) {
        int x0 = curx1;
        int x1 = curx2;
        drawHorizontalLine( x0, x1, scanlineY, 77);
        curx1 += invslope1;
        curx2 += invslope2;
    }
}
void swap( long *a, long *b) {
    *a ^= *b;
    *b ^= *a;
    *a ^= *b;
}
void polygonFill(long x0, long y0, long x1, long y1, long x2, long y2) {
    float fx0, fy0, fx1, fy1, fx2, fy2;
    if(y0>y1) {
        swap( &x0, &x1);
        swap( &y0, &y1);
    }
    if(y0>y2) {
        swap( &x0, &x2);
        swap( &y0, &y2);
    }
    if(y1>y2) {
        swap( &x1, &x2);
        swap( &y1, &y2);
    }
    fx0 = FIXED_TO_FLOAT(x0);
    fy0 = FIXED_TO_FLOAT(y0);
    fx1 = FIXED_TO_FLOAT(x1);
    fy1 = FIXED_TO_FLOAT(y1);
    fx2 = FIXED_TO_FLOAT(x2);
    fy2 = FIXED_TO_FLOAT(y2);
    filling(fx0, fy0, fx1, fy1, fx2, fy2);
}
void polygon(long x0, long y0, long x1, long y1, long x2, long y2) {
    float fx0, fy0, fx1, fy1, fx2, fy2;
    if(y0>y2) {
        swap( &x0, &x2);
        swap( &y0, &y2);
    }
    if(y1>y2) {
        swap( &x1, &x2);
        swap( &y1, &y2);
    }
    if(y0>y1) {
        swap( &x0, &x1);
        swap( &y0, &y1);
    }
    fx0 = FIXED_TO_FLOAT(x0);
    fy0 = FIXED_TO_FLOAT(y0);
    fx1 = FIXED_TO_FLOAT(x1);
    fy1 = FIXED_TO_FLOAT(y1);
    fx2 = FIXED_TO_FLOAT(x2);
    fy2 = FIXED_TO_FLOAT(y2);
    line(fx0, fy0, fx1, fy1);
    line(fx0, fy0, fx2, fy2);
    line(fx1, fy1, fx2, fy2);
}
struct Vec transform(struct Vec A) {
    struct Vec B;
    B.x = 160.0f + ( 160.0f * A.x / A.z );
    B.y = 100.0f + ( 100.0f * A.y / A.z );
    return B;
}
float pyor=0.0f;
void demo() {
    word e;
    float i;
    word T = *my_clock;
    struct Vec z =  VecCreateF( 160.0f, 100.0f, 0.0f);
    pyor = (float)(T)/100.0f;
    for(i=0.0; i<3.0f; i++){
        struct Vec a = VecCreateF( 64.0f,-32.0f, 0.0f);
        struct Vec b = VecCreateF(-64.0f,-64.0f, 0.0f);
        struct Vec c = VecCreateF( i, 64.0f, 0.0f);
        a = rot2D( a, pyor+i/11.0f );
        b = rot2D( b, pyor+i/11.0f );
        c = rot2D( c, pyor+i/11.0f );
        a = add( a, z);
        b = add( b, z);
        c = add( c, z);
        polygonFill(a.x, a.y, b.x, b.y, c.x, c.y);
        polygon(a.x, a.y, b.x, b.y, c.x, c.y);
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

    set_mode(VGA_256_COLOR_MODE);
    while ( !kbhit() ) {
        displayImage(background);
        demo();
        wait_for_retrace();
        UpdateInfo();
    }

    midasMagicDies();
    set_mode(TEXT_MODE);
    return 0;
}

