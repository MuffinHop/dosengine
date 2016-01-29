#define VIDEO_INT           0x10      /* the BIOS video interrupt. */
#define SET_MODE            0x00      /* BIOS func to set the video mode. */
#define VGA_256_COLOR_MODE  0x13      /* use to set 256-color mode. */
#define EGAMODE             0x0d 
#define TEXT_MODE           0x03      /* use to set 80x25 text mode. */

#define PALETTE_INDEX       0x03c8
#define PALETTE_DATA        0x03c9
#define INPUT_STATUS        0x03da
#define VRETRACE            0x08

#define CRTC_INDEX          0x03d4    /* VGA CRT controller */
#define CRTC_DATA           0x03d5
#define SCREEN_WIDTH        320       /* width in pixels of mode 0x13 */
#define SCREEN_HEIGHT       200       /* height in pixels of mode 0x13 */
#define NUM_COLORS          256       /* number of colors in mode 0x13 */

#define NEW(c) (malloc((c)))
/* CRT controller registers */
#define HIGH_ADDRESS 0x0C
#define LOW_ADDRESS  0x0D

typedef unsigned char  byte;
typedef unsigned short word;
typedef unsigned long  dword;

byte *VGA=(byte *)0xA0000;        /* this points to video memory. */
word *my_clock=(word *)0x0000046C;    /* this points to the 18.2hz system*/

byte videobuffer[2][320*200];


word time;
word start;
float ftime;

byte chunkymode=1;

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
#include "src\loader\loader.h"

unsigned        position;               /* Current position */
unsigned        pattern;                /* Current pattern number */
unsigned        row;                    /* Current row number */
int             syncInfo;               /* Music synchronization info */

char            *moduleName = "DEMO.XM";


static int      border = 0;

MIDASmodule module;
MIDASmodulePlayHandle playHandle;

volatile long frameCount = 0;
static unsigned refreshRate;        /* Display refresh rate */


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