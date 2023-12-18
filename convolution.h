#ifndef CONVOLUTION_H
#define CONVOLUTION_H

#include <stdio.h>
#include <stdlib.h>

// Structure pour stocker un filtre de convolution
// On représente ici un filtre carré de côté impair: l'attribut size représente le coté du carré
typedef struct {
    double **valeurs;
    unsigned int cote;
} Filtre;


void verifyFilter(Filtre filtre);
void freeFilter(Filtre filtre);
double sommeFiltre(Filtre filtre);
PGMImage convolution(PGMImage image, Filtre filtre, bool nettoyage);
void nettoyerBords(PGMImage imageOrigine, PGMImage imageFiltree, unsigned int decalage);
Filtre filtreGaussien(unsigned int n, double sigma);
double gaussian(int x, int y, double sigma);
Filtre filtreMoyenneur(unsigned int n);
PGMImage convFiltreMoyenneur(PGMImage image, unsigned int taille_filtre);
PGMImage convFiltreGaussien(PGMImage image, unsigned int taille_filtre, double sigma);
int comparer(const void *a, const void *b);
double mediane(int *liste, int taille);
PGMImage transfo_filtre_median(PGMImage image, unsigned int taille_filtre, bool nettoyage);
PGMImage transfo_filtre_max(PGMImage image, unsigned int taille_filtre, bool nettoyage);
PGMImage transfo_filtre_min(PGMImage image, unsigned int taille_filtre, bool nettoyage);


#endif /* CONVOLUTION_H */