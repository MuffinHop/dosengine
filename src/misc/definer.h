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


float currentTime;
float prevTime;
float startTime;
float passedTime;

#include <stdio.h>
#include <stdlib.h>
#include <dos.h>
#include <conio.h>
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
#include "src\3d\raster.h"
#include "src\3d\matrix.h"
#include "src\3d\vector.h"
#include "src\connection\connect.h"

#define FIXED_TO_FLOAT(x) ((float)x >> 16);
#define FLOAT_TO_FIXED(f) (long)(f * 65536);
#define INT_TO_FIXED(x) (long)(x << 16)
#define FIXED_TO_INT(x) (int) ( (long)(x >> 16) & 0x0000ffff)

/*  long long -> turns into 64-bit
    Any 32-bit CPU or even MMX will go ape shit over this
    and lose any speed gain from fixed*/
//#define FIXEDMUL(x, y) ((long long)(x)*(long long)(y)) / 65536

/*  Not super accurate but good enough*/
#define FIXEDMUL(x, y) ((x>>8)*(y>>8))

//#define FIXEDDIV(x, y) (((long long)(x)<<16)/y)
#define FIXEDDIV(x, y) ((x<<5)/y)<<11 
inline long FIXCEIL( long a) {
    long decimal, integer;
    decimal = a & 0x0000ffff;
    if(decimal != 0) {
        integer = a & 0xffff0000;
        integer += 0xffff0000;
        return integer;
    }
    return a;
}
long SIN1024[1024];
long COS1024[1024];
void calcSIN() {
    float i=0.0f;
    for(i=0.0f; i<1024.0f; i++) {
        float sini = sin(3.14151591f * 2.0f * i/1024.0f);
        SIN1024[(int)i] = FLOAT_TO_FIXED(sini);
    }
}
void calcCOS() {
    float i=0.0f;
    for(i=0.0f; i<1024.0f; i++) {
        float cosi = cos(3.14151591f * 2.0f * i/1024.0f);
        COS1024[(int)i] = FLOAT_TO_FIXED(cosi);
    }
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