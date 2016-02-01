#define VIDEO_INT           0x10      /* the BIOS video interrupt. */
#define SET_MODE            0x00      /* BIOS func to set the video mode. */
#define VGA_256_COLOR_MODE  0x13      /* use to set 256-color mode. */
#define TEXT_MODE           0x03      /* use to set 80x25 text mode. */

#define SC_INDEX            0x03c4    /* VGA sequence controller */
#define SC_DATA             0x03c5
#define PALETTE_INDEX       0x03c8    /* VGA digital-to-analog converter */
#define PALETTE_DATA        0x03c9
#define GC_INDEX            0x03ce    /* VGA graphics controller */
#define GC_DATA             0x03cf
#define CRTC_INDEX          0x03d4    /* VGA CRT controller */
#define CRTC_DATA           0x03d5
#define INPUT_STATUS        0x03da

#define MAP_MASK            0x02      /* Sequence controller registers */
#define ALL_PLANES          0xff02
#define MEMORY_MODE         0x04

#define LATCHES_ON          0x0008    /* Graphics controller registers */
#define LATCHES_OFF         0xff08

#define HIGH_ADDRESS        0x0C      /* CRT controller registers */
#define LOW_ADDRESS         0x0D
#define UNDERLINE_LOCATION  0x14
#define MODE_CONTROL        0x17

#define DISPLAY_ENABLE      0x01      /* VGA input status bits */
#define VRETRACE            0x08

#define SCREEN_WIDTH        320       /* width in pixels of mode 0x13 */
#define SCREEN_HEIGHT       200       /* height in pixels of mode 0x13 */
#define SCREEN_SIZE         (word)(SCREEN_WIDTH*SCREEN_HEIGHT)
#define NUM_COLORS          256       /* number of colors in mode 0x13 */

#define BITMAP_WIDTH        32
#define BITMAP_HEIGHT       25
#define ANIMATION_FRAMES    24
#define TOTAL_FRAMES        140
#define VERTICAL_RETRACE              /* comment out this line for more accurate timing */

#define PALETTE_INDEX       0x03c8
#define PALETTE_DATA        0x03c9
#define INPUT_STATUS        0x03da
#define VRETRACE            0x08

#define NEW(c) (malloc((c)))
/* CRT controller registers */
#define HIGH_ADDRESS 0x0C
#define LOW_ADDRESS  0x0D

typedef unsigned char  byte;
typedef unsigned short word;
typedef unsigned long  dword;

byte *VGA=(byte *)0xA0000;        /* this points to video memory. */
word *my_clock=(word *)0x0000046C;    /* this points to the 18.2hz system*/

byte videobuffer[320*200*2];

word time;
word start;
float ftime;

byte chunkymode = 1;
byte chainmode = 0;
byte extrememode = 0;

unsigned        position;               /* Current position */
unsigned        pattern;                /* Current pattern number */
unsigned        row;                    /* Current row number */
int             syncInfo;               /* Music synchronization info */

char            *moduleName = "DEMO.XM";


static int      border = 0;

volatile long   frameCount = 0;

/* Display refresh rate */
static unsigned refreshRate; 
unsigned int    visible_page        =   0;
unsigned int    non_visible_page    =   320*200/4;
word            temp;

byte* extremeBuffer[320*66*3];

#include <stdio.h>
#include <stdlib.h>
#include <dos.h>
#include <mem.h>
#include <math.h>
#include "src\player\magic.h"
#include "src\beeper\beeper.h"
#include "src\timing\timing.h"
#include "src\memory\buffer.h"
#include "src\misc\midasdll.h"
#include "src\image\vgaimg.h"
#include "src\image\image.h"
#include "src\video\video.h"
#include "src\loader\loader.h"
#include "src\3d\horzline.h"

#define FIXED_TO_FLOAT(x) ((float)x / 65536);
#define FLOAT_TO_FIXED(f) (long)(f*65536);

float SIN1024[1024];
float COS1024[1024];
void calcSIN() {
    float i=0.0f;
    for(i=0.0f; i<1024.0f; i++) {
        float sini = sin(3.14151591f * 2.0f * i/1024.0f);
        SIN1024[(int)i] = sini;
    }
}
void calcCOS() {
    float i=0.0f;
    for(i=0.0f; i<1024.0f; i++) {
        float cosi = cos(3.14151591f * 2.0f * i/1024.0f);
        COS1024[(int)i] = cosi;
    }
}

struct Vec {
   long x, y, z;
}; 
struct Vec VecCreateF( float x_, float y_, float z_) {
    struct Vec r_;
    r_.x = FLOAT_TO_FIXED(x_);
    r_.y = FLOAT_TO_FIXED(y_);
    r_.z = FLOAT_TO_FIXED(z_);
    return r_;
};
struct Vec VecCreateL( long x_, long y_, long z_) {
    struct Vec r_;
    r_.x = x_;
    r_.y = y_;
    r_.z = z_;
    return r_;
};
struct Vec add( struct Vec A, struct Vec B) {
    return VecCreateL(A.x+B.x,A.y+B.y,A.z+B.z);
}
struct Vec sub( struct Vec A, struct Vec B) {
    return VecCreateL(A.x-B.x,A.y-B.y,A.z-B.z);
}
struct Vec mul( struct Vec A, struct Vec B) {
    return VecCreateL(A.x*B.x,A.y*B.y,A.z*B.z);
}
struct Vec norm( struct Vec A) {
    float L = sqrt( A.x*A.x + A.y*A.y + A.z * A.z);
    return VecCreateL(A.x/L,A.y/L,A.z/L);
}
float dot( struct Vec A, struct Vec B) {
    return A.x*B.x+A.y*B.y+A.z*B.z;
}
struct Vec cross( struct Vec A, struct Vec B) {
    return VecCreateL(A.y*B.z-A.z*B.y,A.z*B.x-A.x*B.z,A.x*B.y-A.y*B.x);
}
struct Vec rot2D( struct Vec A, float rad ) {
    long X, Y;
    float sina = SIN1024[(int)(rad*1024.0f)&1023];
    float cosa = COS1024[(int)(rad*1024.0f)&1023];
    X = A.x * cosa - A.y * sina;
    Y = A.x * sina + A.y * cosa;
    return VecCreateL(X, Y, A.z);
}




MIDASmodule module;
MIDASmodulePlayHandle playHandle;

/* 
    Keep array small, 
    because easily uses a lot of memory. 
    For example 320x200x16 > 2Mb. Regular pentiums had 8-16Mb.*/
struct Image images[16]; 

void vgaWaitVR(void);
#pragma aux vgaWaitVR = \
    "mov    dx,03DAh" \
    "vr:" \
    "in     al,dx" \
    "test   al,8" \
    "jz     vr" \
    modify exact [eax edx];

void vgaWaitNoVR(void);
#pragma aux vgaWaitNoVR = \
    "mov    dx,03DAh" \
    "nvr:" \
    "in     al,dx" \
    "test   al,8" \
    "jnz    nvr" \
    modify exact [eax edx];

void vgaWaitDE(void);
#pragma aux vgaWaitDE = \
    "mov    dx,03DAh" \
    "de:" \
    "in     al,dx" \
    "test   al,1" \
    "jnz    de" \
    modify exact [eax edx];

void vgaSetBorder(int color);
#pragma aux vgaSetBorder = \
    "mov    dx,03DAh" \
    "in     al,dx" \
    "mov    dx,03C0h" \
    "mov    al,31h" \
    "out    dx,al" \
    "mov    al,bl" \
    "out    dx,al" \
    parm [ebx] \
    modify exact [eax edx];