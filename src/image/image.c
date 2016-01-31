#include "src\misc\definer.h"
void displayImage(struct Image img) {
	int i,j;
    int width = img.width;
    for(i=0;i<256;i++) {
        outp(0x03c8, (byte)i);
        outp(0x03c9, img.palettebuffer [i * 3 + 0]);
        outp(0x03c9, img.palettebuffer [i * 3 + 1]);
        outp(0x03c9, img.palettebuffer [i * 3 + 2]);
    }
    for(j=0; j<200; j++) {
        for(i=0; i<320; i++) {
            videobuffer[i+j*320] = img.pixelbuffer[min(i,width)+j*width];
        }
    }
}
struct Image loadImage(const char* file) {
    int width = 320;
    int height = 200;
    struct Image img;

    img.pixelbuffer = malloc(320 * 200);
    img.palettebuffer = malloc(256 * 3);
    PIX_LoadGIF(file, img.pixelbuffer, img.palettebuffer , &width, &height);
    
    img.width = width;
    img.height = height;
    return img;
}