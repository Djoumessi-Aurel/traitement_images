#include <stdio.h>
#include <stdlib.h>


double luminance(PGMImage image);
double contraste(PGMImage image);
PGMImage transfo_lineaire(PGMImage image);
PGMImage transfo_lineaire_saturation(PGMImage image, int Smin, int Smax);
PGMImage correction_gamma(PGMImage image, double gamma);
PGMImage correction_dynamique(PGMImage image, int valeur_max);
PGMImage egalisation_hist(PGMImage image);
PGMImage etLogique(PGMImage image1, PGMImage image2);
PGMImage ouLogique(PGMImage image1, PGMImage image2);
PGMImage addition(PGMImage image1, PGMImage image2);
PGMImage soustraction(PGMImage image1, PGMImage image2);
PGMImage multiplication(PGMImage image, double k);
PGMImage redimensionner(PGMImage image, unsigned int newLargeur, unsigned int newHauteur);
