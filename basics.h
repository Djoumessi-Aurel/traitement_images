#include <stdio.h>
#include <stdlib.h>

// Structure pour stocker l'image PGM
typedef struct {
    unsigned char **pixels;
    int width;
    int height;
} PGMImage;


PGMImage readPGM(const char *filename);
PGMImage readAsciiPGM(const char *filename);
PGMImage readBinaryPGM(const char *filename);
void writeAsciiPGM(const char *filename, PGMImage image);
void writeBinaryPGM(const char *filename, PGMImage image);
void freeImage(PGMImage image);
PGMImage copyImage(PGMImage image, bool copy_pixels);
PGMImage createImage(int width, int height);
PGMImage createImageWithColor(int width, int height, unsigned char valeur_pixel);
