#include "src\misc\definer.h"
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
void swap( long *a, long *b) {
    *a ^= *b;
    *b ^= *a;
    *a ^= *b;
}

void fillingFixed(long x1, long y1, long x2, long y2, long x3, long y3) {
    long invslope1, invslope2, curx1, curx2;
    long scanlineY;
    invslope1 = FIXEDDIV( (x2 - x1) , (y2 - y1) );
    invslope2 = FIXEDDIV( (x3 - x1) , (y3 - y1) );

    curx1 = x1;
    curx2 = x1;
    scanlineY = y1;
    if(y1 != y2)
    for ( scanlineY = FIXED_TO_INT(y1); scanlineY < FIXED_TO_INT(y2); scanlineY++) {
        int x0 = FIXED_TO_INT(curx1);
        int x1 = FIXED_TO_INT(curx2);
        if(scanlineY>0 && scanlineY<200)
            drawHorizontalLine( x0, x1, scanlineY, 33);
        curx1 += invslope1;
        curx2 += invslope2;
        if(scanlineY>200)
            break;
    }

    if(y1 == y2) curx2 = x2;

    invslope1 = FIXEDDIV( (x3 - curx1) , (y3 - INT_TO_FIXED ( scanlineY ) ) );
    invslope2 = FIXEDDIV( (x3 - curx2) , (y3 - INT_TO_FIXED ( scanlineY ) ) );

    curx1 = curx1;
    curx2 = curx2;
    if(y2 != y3)
    for ( scanlineY = scanlineY; scanlineY < FIXED_TO_INT(y3); scanlineY++) {
        int x0 = FIXED_TO_INT(curx1);
        int x1 = FIXED_TO_INT(curx2);
        if(scanlineY>0 && scanlineY<200)
            drawHorizontalLine( x0, x1, scanlineY, 77);
        curx1 += invslope1;
        curx2 += invslope2;
        if(scanlineY>200)
            break;
    }
}
void polygonFillFixed(long x0, long y0, long x1, long y1, long x2, long y2) {
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
    x0 = FIXCEIL(x0);
    y0 = FIXCEIL(y0);
    x1 = FIXCEIL(x1);
    y1 = FIXCEIL(y1);
    x2 = FIXCEIL(x2);
    y2 = FIXCEIL(y2);
    fillingFixed(x0, y0, x1, y1, x2, y2);
}

void polygon(long x0, long y0, long x1, long y1, long x2, long y2) {
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
    line(FIXED_TO_INT(x0), FIXED_TO_INT(y0), FIXED_TO_INT(x1), FIXED_TO_INT(y1));
    line(FIXED_TO_INT(x0), FIXED_TO_INT(y0), FIXED_TO_INT(x2), FIXED_TO_INT(y2));
    line(FIXED_TO_INT(x1), FIXED_TO_INT(y1), FIXED_TO_INT(x2), FIXED_TO_INT(y2));
}
struct Vec transform(struct Vec A) {
    struct Vec B;
    B.x = 160.0f + ( 160.0f * A.x / A.z );
    B.y = 100.0f + ( 100.0f * A.y / A.z );
    return B;
}