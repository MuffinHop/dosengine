struct Image {
    unsigned char *pixelbuffer;
    unsigned char *palettebuffer;
    unsigned int width;
    unsigned int height;
};
void displayImage(struct Image img);
struct Image loadImage(const char* file);
struct Image background;