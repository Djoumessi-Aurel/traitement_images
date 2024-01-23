#include <math.h>
#include "contours.h"
#include "basics.h"


// Fonction pour l'érosion d'une image PGM avec un kernel donné
PGMImage erosion(PGMImage image, int kernel[3][3]) {
    PGMImage result = createImage(image.width, image.height);

    // Appliquer l'érosion
    for (int i = 1; i < image.height - 1; ++i) {
        for (int j = 1; j < image.width - 1; ++j) {
            unsigned char min = 255; // Initialiser à une valeur élevée

            // Parcourir le kernel
            for (int m = -1; m <= 1; ++m) {
                for (int n = -1; n <= 1; ++n) {
                    if (kernel[m + 1][n + 1] == 1) {
                        if (image.pixels[i + m][j + n] < min) {
                            min = image.pixels[i + m][j + n];
                        }
                    }
                }
            }

            // Affecter la valeur minimale au résultat
            result.pixels[i][j] = min;
        }
    }

    return result;
}


// Fonction pour la dilatation d'une image PGM avec un kernel donné
PGMImage dilatation(PGMImage image, int kernel[3][3]) {
    PGMImage result = createImage(image.width, image.height);

    // Appliquer la dilatation
    for (int i = 1; i < image.height - 1; ++i) {
        for (int j = 1; j < image.width - 1; ++j) {
            unsigned char max = 0; // Initialiser à une valeur basse

            // Parcourir le kernel
            for (int m = -1; m <= 1; ++m) {
                for (int n = -1; n <= 1; ++n) {
                    if (kernel[m + 1][n + 1] == 1) {
                        if (image.pixels[i + m][j + n] > max) {
                            max = image.pixels[i + m][j + n];
                        }
                    }
                }
            }

            // Affecter la valeur maximale au résultat
            result.pixels[i][j] = max;
        }
    }

    return result;
}


// Fonction pour l'ouverture (érosion suivie de dilatation)
PGMImage ouverture(PGMImage image, int erosionKernel[3][3], int dilatationKernel[3][3]) {
    // Appliquer l'érosion
    PGMImage resultatErosion = erosion(image, erosionKernel);

    // Appliquer la dilatation sur le résultat de l'érosion
    PGMImage resultatOuverture = dilatation(resultatErosion, dilatationKernel);

    // Libérer la mémoire utilisée par l'image résultante de l'érosion
    freeImage(resultatErosion);

    return resultatOuverture;
}


// Fonction pour l'opération de fermeture (dilatation suivie de érosion)
PGMImage fermeture(PGMImage image, int kernel[3][3]) {
    PGMImage dilatee = dilatation(image, kernel);
    PGMImage fermetureResult = erosion(dilatee, kernel);

    freeImage(dilatee);

    return fermetureResult;
}
