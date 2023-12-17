#include <math.h>
#include "convolution.h"
#include "basics.h"

#ifndef M_PI
#define M_PI 3.14159265358979323846264338
#endif


// Vérifie que le filtre satisfait les conditions nécessaires pour servir de filtre de convolution
void verifyFilter(Filtre filtre){
    if(filtre.cote < 3 || filtre.cote % 2 == 0){
        fprintf(stderr, "Vérification du filtre: le filtre doit avoir un côté impair et supérieur à 1!\n");
        exit(1);
    }
}

// Libération de la mémoire occupée par le filtre
void freeFilter(Filtre filtre){
    for (int i = 0; i < filtre.cote; ++i) {
        free(filtre.valeurs[i]);
    }
    free(filtre.valeurs);
}


// Somme des valeurs d'un filtre
double sommeFiltre(Filtre filtre){
    double S = 0;
    for (int i = 0; i < filtre.cote; ++i) {
        for (int j = 0; j < filtre.cote; ++j) {
            S += filtre.valeurs[i][j];
        }
    }

    return S;
}


// Convolution d'une image par un filtre
PGMImage convolution(PGMImage image, Filtre filtre, bool nettoyage){
    verifyFilter(filtre);

    PGMImage result = createImageWithColor(image.width, image.height, 0);
    double temp = 0, S = sommeFiltre(filtre);
    unsigned int d = filtre.cote / 2;

    for (int i = d; i < image.height - d; ++i) {
        for (int j = d; j < image.width - d; ++j) {
            // Calcul de I'[i][j]
            temp = 0;
            for (int u = 0; u < filtre.cote; ++u) {
                for (int v = 0; v < filtre.cote; ++v) {
                    temp += image.pixels[i - d + u][j - d + v] * filtre.valeurs[u][v];
                }
            }
            result.pixels[i][j] = temp/S;
        }
    }

    if(nettoyage) nettoyerBords(image, result, d);

    return result;
}


// Nettoie les bords de l'image filtrée après convolution, en conservant les mêmes valeurs de pixels que l'image d'origine
// decalage représente le décalage entre les bords de l'image et la zone convoluée
void nettoyerBords(PGMImage imageOrigine, PGMImage imageFiltree, unsigned int decalage){
    for (int i = 0; i < imageOrigine.height; ++i) {
        for (int j = 0; j < imageOrigine.width; ++j) {
            if(i < decalage || i >= imageOrigine.height - decalage || j < decalage || j >= imageOrigine.width - decalage){
                imageFiltree.pixels[i][j] = imageOrigine.pixels[i][j];
            }
        }
    }
}


// Calcule le filtre gaussien de taille n x n avec la valeur de sigma donnée
Filtre filtreGaussien(unsigned int n, double sigma){
    Filtre filtre;
    filtre.cote = n;
    filtre.valeurs = (double**)malloc(n * sizeof(double*));

    if (filtre.valeurs == NULL) {
        fprintf(stderr, "Fonction filtreGaussien: Erreur d'allocation de mémoire!\n");
        exit(1);
    }

    int centre = n / 2;
    for (int i = 0; i < n; ++i) {
        filtre.valeurs[i] = (double*)malloc(n * sizeof(double));
        if (filtre.valeurs[i] == NULL) {
            fprintf(stderr, "Fonction filtreGaussien: Erreur d'allocation de mémoire!\n");
            exit(1);
        }
        for (int j = 0; j < n; ++j) {
            filtre.valeurs[i][j] = gaussian(i - centre, j - centre, sigma);
        }
    }

    return filtre;
}


// Fonction pour calculer une valeur gaussienne
double gaussian(int x, int y, double sigma) {
    return exp(-(x * x + y * y) / (2 * sigma * sigma)) / (2 * M_PI * sigma * sigma);
}


// Calcule le filtre moyenneur de taille n x n
Filtre filtreMoyenneur(unsigned int n){
    Filtre filtre;
    filtre.cote = n;
    filtre.valeurs = (double**)malloc(n * sizeof(double*));

    if (filtre.valeurs == NULL) {
        fprintf(stderr, "Fonction filtreMoyenneur: Erreur d'allocation de mémoire!\n");
        exit(1);
    }

    for (int i = 0; i < n; ++i) {
        filtre.valeurs[i] = (double*)malloc(n * sizeof(double));
        if (filtre.valeurs[i] == NULL) {
            fprintf(stderr, "Fonction filtreMoyenneur: Erreur d'allocation de mémoire!\n");
            exit(1);
        }
        for (int j = 0; j < n; ++j) {
            filtre.valeurs[i][j] = 1;
        }
    }

    return filtre;
}


// Convolution avec filtre moyenneur
PGMImage convFiltreMoyenneur(PGMImage image, unsigned int taille_filtre){
    Filtre filtre = filtreMoyenneur(taille_filtre);
    verifyFilter(filtre);

    PGMImage result = convolution(image, filtre, true);
    freeFilter(filtre);

    return result;
}


// Convolution avec filtre gaussien
PGMImage convFiltreGaussien(PGMImage image, unsigned int taille_filtre, double sigma){
    Filtre filtre = filtreGaussien(taille_filtre, sigma);
    verifyFilter(filtre);

    PGMImage result = convolution(image, filtre, true);
    freeFilter(filtre);

    return result;
}