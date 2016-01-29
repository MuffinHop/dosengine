struct Image {
    unsigned char *pixelbuffer;
    unsigned char *palettebuffer;
    unsigned int width;
    unsigned int height;
};
void displayImage(struct Image img, unsigned int bufindx);
struct Image loadImage(const char* file);
struct Image background;