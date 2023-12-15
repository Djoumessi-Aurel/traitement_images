#include <math.h>
#include "traitements_base.h"

// Calcule la luminance de l'image
double luminance(PGMImage image){
    
    double luminance = 0;
    for (int i = 0; i < image.height; ++i) {
        for (int j = 0; j < image.width; ++j) {
            luminance += image.pixels[i][j];
        }
    }
    return luminance / (image.height * image.width);
}

// Calcule le contraste de l'image
double contraste(PGMImage image){
    
    double e_x2 = 0, e_x = 0;
    for (int i = 0; i < image.height; ++i) {
        for (int j = 0; j < image.width; ++j) {
            e_x += image.pixels[i][j];
            e_x2 += (image.pixels[i][j] * image.pixels[i][j]);
        }
    }
    int nb = image.height * image.width;
    e_x /= nb;
    e_x2 /= nb;
    return sqrt(e_x2 - e_x * e_x);
}


// Amélioration du contraste par transformation linéaire. Retourne l'image transformée
PGMImage transfo_lineaire(PGMImage image){
    int minI = image.pixels[0][0], maxI = image.pixels[0][0], diff = 0;
    PGMImage resultImage;

    for (int i = 0; i < image.height; ++i) {
        for (int j = 0; j < image.width; ++j) {
            if(image.pixels[i][j] < minI) minI = image.pixels[i][j];
            if(image.pixels[i][j] > maxI) maxI = image.pixels[i][j];
        }
        if(minI == 0 && maxI == 255) break;
    }

    diff = maxI - minI;

    if(diff == 0 || diff == 255){
        resultImage = copyImage(image, true);
    }
    else{
        int Lookup[256];
        for (int i = 0; i < 256; ++i) {
            Lookup[i] = 255 * (i - minI) / diff;
        }

        resultImage = copyImage(image, false);
        for (int i = 0; i < image.height; ++i) {
            for (int j = 0; j < image.width; ++j) {
                resultImage.pixels[i][j] = Lookup[image.pixels[i][j]];
            }
        }
    }

    return resultImage;
}


// Correction gamma. Retourne l'image transformée
PGMImage correction_gamma(PGMImage image, double gamma){
    PGMImage resultImage;

    int Lookup[256];
    for (int i = 0; i < 256; ++i) {
        Lookup[i] = 255 * pow(((double)i)/255, gamma);
    }

    resultImage = copyImage(image, false);
    for (int i = 0; i < image.height; ++i) {
        for (int j = 0; j < image.width; ++j) {
            resultImage.pixels[i][j] = Lookup[image.pixels[i][j]];
        }
    }

    return resultImage;
}


// Correction de la dynamique de l'image (Histogram Stretch). Retourne l'image transformée
PGMImage correction_dynamique(PGMImage image, int valeur_max){
    if(valeur_max < 0 || valeur_max > 255){
        fprintf(stderr, "Histogram stretch: valeur_max doit être comprise entre 0 et 255.\n");
        return copyImage(image, true);
    }

    int minI = image.pixels[0][0], maxI = image.pixels[0][0], diff = 0;
    PGMImage resultImage;

    for (int i = 0; i < image.height; ++i) {
        for (int j = 0; j < image.width; ++j) {
            if(image.pixels[i][j] < minI) minI = image.pixels[i][j];
            if(image.pixels[i][j] > maxI) maxI = image.pixels[i][j];
        }
        if(minI == 0 && maxI == 255) break;
    }

    diff = maxI - minI;

    if(diff == 0){
        resultImage = copyImage(image, true);
    }
    else{
        int Lookup[256];
        for (int i = 0; i < 256; ++i) {
            Lookup[i] = valeur_max * (i - minI) / diff;
        }

        resultImage = copyImage(image, false);
        for (int i = 0; i < image.height; ++i) {
            for (int j = 0; j < image.width; ++j) {
                resultImage.pixels[i][j] = Lookup[image.pixels[i][j]];
            }
        }
    }

    return resultImage;
}


// Egalisation d'histogramme. Retourne l'image transformée
PGMImage egalisation_hist(PGMImage image){
    PGMImage resultImage;
    int nb = image.height * image.width;

    // Calcul de l'histogramme h(i)
    double h[256];
    for (int i = 0; i < image.height; ++i) {
        for (int j = 0; j < image.width; ++j) {
            h[image.pixels[i][j]] ++;
        }
    }

    // Normalisation de l'histogramme
    for (int i = 0; i < 256; ++i) {
        h[i] /= nb;
    }

    // Densité de probablilité normalisé
    double C[256];
    C[0] = h[0];
    for (int i = 1; i < 256; ++i) {
        C[i] = C[i-1] + h[i];
    }

    // Transformation des niveaux de gris de l'image
    resultImage = copyImage(image, false);
    for (int i = 0; i < image.height; ++i) {
        for (int j = 0; j < image.width; ++j) {
            resultImage.pixels[i][j] = C[image.pixels[i][j]] * 255;
        }
    }

    return resultImage;
}


// Opération ET entre deux images
PGMImage etLogique(PGMImage image1, PGMImage image2){
    if(image1.width != image2.width || image1.height != image2.height){
        fprintf(stderr, "Erreur dans \"etLogique\": les deux images n'ont pas les mêmes dimensions.\n");
        exit(1);
    }

    PGMImage result = copyImage(image1, true);
    for (int i = 0; i < result.height; ++i) {
        for (int j = 0; j < result.width; ++j) {
            result.pixels[i][j] &= image2.pixels[i][j];
        }
    }

    return result;
}


// Opération OU entre deux images
PGMImage ouLogique(PGMImage image1, PGMImage image2){
    if(image1.width != image2.width || image1.height != image2.height){
        fprintf(stderr, "Erreur dans \"etLogique\": les deux images n'ont pas les mêmes dimensions.\n");
        exit(1);
    }

    PGMImage result = copyImage(image1, true);
    for (int i = 0; i < result.height; ++i) {
        for (int j = 0; j < result.width; ++j) {
            result.pixels[i][j] |= image2.pixels[i][j];
        }
    }

    return result;
}


// Addition entre deux images
PGMImage addition(PGMImage image1, PGMImage image2){
    if(image1.width != image2.width || image1.height != image2.height){
        fprintf(stderr, "Erreur dans \"etLogique\": les deux images n'ont pas les mêmes dimensions.\n");
        exit(1);
    }

    PGMImage result = copyImage(image1, false);
    for (int i = 0; i < result.height; ++i) {
        for (int j = 0; j < result.width; ++j) {
            result.pixels[i][j] = __min(255, image1.pixels[i][j] + image2.pixels[i][j]);
        }
    }

    return result;
}


// Soustraction entre deux images
PGMImage soustraction(PGMImage image1, PGMImage image2){
    if(image1.width != image2.width || image1.height != image2.height){
        fprintf(stderr, "Erreur dans \"etLogique\": les deux images n'ont pas les mêmes dimensions.\n");
        exit(1);
    }

    PGMImage result = copyImage(image1, false);
    for (int i = 0; i < result.height; ++i) {
        for (int j = 0; j < result.width; ++j) {
            result.pixels[i][j] = __max(0, image1.pixels[i][j] - image2.pixels[i][j]);
        }
    }

    return result;
}


// Mutiplication d'une image par un réel positif
PGMImage multiplication(PGMImage image, double k){
    if(k < 0){
        fprintf(stderr, "Erreur dans \"multiplication\": Impossible de multiplier l'image par un nombre négatif.\n");
        exit(1);
    }

    PGMImage result = copyImage(image, false);
    for (int i = 0; i < result.height; ++i) {
        for (int j = 0; j < result.width; ++j) {
            result.pixels[i][j] = __min(255, k * image.pixels[i][j]);
        }
    }

    return result;
}


// Produit une version redimensionnée de 'image' ayant une largeur 'newLargeur' et une hauteur 'newHauteur'
PGMImage redimensionner(PGMImage image, unsigned int newLargeur, unsigned int newHauteur){
    if(newLargeur == 0 || newHauteur == 0){
        fprintf(stderr, "Erreur dans \"redimensionner\": les nouvelles dimensions doivent être strictement positives!\n");
        exit(1);
    }

    double fLargeur = (double) newLargeur / image.width;
    double fHauteur = (double) newHauteur / image.height;

    PGMImage result = createImage(newLargeur, newHauteur);
    
    for (int i = 0; i < result.height; ++i) {
        for (int j = 0; j < result.width; ++j) {
            result.pixels[i][j] = image.pixels[(int)(i/fHauteur)][(int)(j/fLargeur)];
        }
    }

    return result;
}