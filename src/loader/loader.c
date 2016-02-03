#include "src\misc\definer.h"
int PIX_LoadGIF(const char* file, unsigned char *outpix, unsigned char * pal, int* width, int* height);
void loader() {
    int i,f,k=0;
    int width = 320;
    int height = 200;
    struct Image loadingimage;

    loadingimage.pixelbuffer = malloc(320 * 200);
    loadingimage.palettebuffer = malloc(256 * 3);

    PIX_LoadGIF("img\\loading.gif", loadingimage.pixelbuffer, loadingimage.palettebuffer , &width, &height);
    
    loadingimage.width = width;
    loadingimage.height = height;
    midasMagicStart();

    set_mode(VGA_256_COLOR_MODE);

    for(i=0; i<256; i++) {
        outp(0x03c8, (byte)i);
        outp(0x03c9, loadingimage.palettebuffer [i * 3 + 0] );
        outp(0x03c9, loadingimage.palettebuffer [i * 3 + 1] );
        outp(0x03c9, loadingimage.palettebuffer [i * 3 + 2] );
    }
    for(f=0; f<200; f++) {
        for(i=0; i<320; i++) {
            videobuffer[i+f*320] = loadingimage.pixelbuffer[min(i,width)+f*width];
        }
    }

    wait_for_retrace();


    /*PRECALCULATION STARTS FROM HERE*/
    /*PRECALCULATION STARTS FROM HERE*/
    /*PRECALCULATION STARTS FROM HERE*/
    /*PRECALCULATION STARTS FROM HERE*/
    /*PRECALCULATION STARTS FROM HERE*/

    calcSIN();
    calcCOS();

    /*PRECALCULATION ENDS HERE*/
    /*PRECALCULATION ENDS HERE*/
    /*PRECALCULATION ENDS HERE*/
    /*PRECALCULATION ENDS HERE*/
    /*PRECALCULATION ENDS HERE*/

    /*LOADING STARTS FROM HERE*/
    /*LOADING STARTS FROM HERE*/
    /*LOADING STARTS FROM HERE*/
    /*LOADING STARTS FROM HERE*/
    /*LOADING STARTS FROM HERE*/

    background = loadImage("img\\backgrou.gif");

    if ( (module = MIDASloadModule(moduleName)) == NULL )
        MIDASerror();
                                                                                           

    /*LOADING ENDS HERE*/
    /*LOADING ENDS HERE*/
    /*LOADING ENDS HERE*/
    /*LOADING ENDS HERE*/
    /*LOADING ENDS HERE*/




    /*Fade out from loading screen*/
    set_mode(VGA_256_COLOR_MODE);  

    for(f=0;f<256;f+=4){
        for(i=0;i<256;i++){
            outp(0x03c8, (byte)i);
            outp(0x03c9, max(loadingimage.palettebuffer[i * 3 + 0]-f,0));
            outp(0x03c9, max(loadingimage.palettebuffer[i * 3 + 1]-f,0));
            outp(0x03c9, max(loadingimage.palettebuffer[i * 3 + 2]-f,0));
        }
        wait_for_retrace();
    }
    for(i=0; i<320*200; i++) {
        videobuffer[i] = 0;
    }
    wait_for_retrace();
}