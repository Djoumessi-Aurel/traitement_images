#ifndef CONTOURS_H
#define CONTOURS_H

#include <stdio.h>
#include <stdlib.h>

PGMImage erosion(PGMImage image, int kernel[3][3]);
PGMImage dilatation(PGMImage image, int kernel[3][3]);
PGMImage ouverture(PGMImage image, int erosionKernel[3][3], int dilatationKernel[3][3]);
PGMImage fermeture(PGMImage image, int kernel[3][3]);


#endif /* CONTOURS_H */