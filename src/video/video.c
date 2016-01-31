#include "src\misc\definer.h"
/*Set video mode*/
void set_mode(byte mode) {
  union REGS regs;
  if(mode == VGA_256_COLOR_MODE) {
  	chunkymode = 1;
  } else {
  	chunkymode = 0;
  }
  regs.h.ah = SET_MODE;
  regs.h.al = mode;
  int386(VIDEO_INT, &regs, &regs);
}
/*turn on chain mode*/
void chainMode() {
  set_mode(VGA_256_COLOR_MODE);  
	outp(SC_INDEX, MEMORY_MODE);
	outp(SC_DATA, 0x06);
	outp(CRTC_INDEX, UNDERLINE_LOCATION);
	outp(CRTC_DATA, 0x00);
	outp(CRTC_INDEX, MODE_CONTROL);
	outp(CRTC_DATA, 0xe3);
	chainmode = 1;
  chunkymode = 0;
}
/*Set plane(s), use only in chainmode*/
void setPlane( byte planes) {
  outp(0x3c4, 0x02);
  outp(0x3c5, planes);
}
/*extreme mode*/
void extremeMode() {
  byte i;
  set_mode(VGA_256_COLOR_MODE);  
	chainMode();
	extrememode = 1;

  for(i=0;i<64;i++) {
    outp(0x03c8, i);
    outp(0x03c9, i*4);
    outp(0x03c9, 0);
    outp(0x03c9, 0);
  }
  for(i=0;i<64;i++){
    outp(0x03c8, i+64);
    outp(0x03c9, 0);
    outp(0x03c9, i*4);
    outp(0x03c9, 0);
  }
  for(i=0;i<64;i++){
    outp(0x03c8, i+128);
    outp(0x03c9, 0);
    outp(0x03c9, 0);
    outp(0x03c9, i*4);
  }
  for(i=0;i<64;i++){
    outp(0x03c8, i+192);
    outp(0x03c9, i*4);
    outp(0x03c9, i*4);
    outp(0x03c9, min(i*6+8,255));
  }
}
/*Render Extreme Mode*/
void extremeRender() {
	int x, y;

  long vgaIndex = 0;
  long extremeIndex = 0;

  setPlane( 15);

  for(y=0; y<67; y++) {
    for(x=0; x<80; x++) {
      byte red = extremeBuffer[ extremeIndex ];
      extremeIndex += 3;  /* jump 3 colors*/
      VGA[non_visible_page+vgaIndex] = red;
      vgaIndex++;
    }
    vgaIndex+=160;
  }
  vgaIndex = 80;
  extremeIndex = 1;
  for(y=0; y<67; y++) {
    for(x=0; x<80; x++) {
      byte green = extremeBuffer[ extremeIndex ];
      extremeIndex += 3;  /* jump 3 colors*/
      VGA[non_visible_page+vgaIndex] = 64+green;
      vgaIndex++;
    }
    vgaIndex+=160;
  }
  vgaIndex = 160;
  extremeIndex = 2;
  for(y=0; y<66; y++) {
    for(x=0; x<80; x++) {
      byte blue = extremeBuffer[ extremeIndex ];
      extremeIndex += 3;  /* jump 3 colors*/
      VGA[non_visible_page+vgaIndex] = 128+blue;
      vgaIndex++;
    }
    vgaIndex+=160;
  }

  setPlane( 4);
  vgaIndex = 80;
  extremeIndex = 0;
  for(y=0; y<66; y++) {
    for(x=0; x<80; x++) {
      byte red = extremeBuffer[ extremeIndex++ ];
      byte green = extremeBuffer[ extremeIndex++ ];
      byte blue = extremeBuffer[ extremeIndex++ ];
      byte mix = red+green;
      mix = mix >> 1;
      mix += blue;
      mix = mix >> 1;
      mix = mix&63;

      VGA[non_visible_page+vgaIndex] = 192+mix;
      vgaIndex++;
    }
    vgaIndex+=160;
  }
}