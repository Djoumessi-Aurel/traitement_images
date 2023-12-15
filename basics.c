#include <stdbool.h>
#include "basics.h"

// Fonction pour lire un fichier PGM (ascii/P2 ou binaire/P5) et générer la matrice de pixels
PGMImage readPGM(const char *filename) {
    PGMImage image;
    FILE *file = fopen(filename, "r");

    if (file == NULL) {
        fprintf(stderr, "Impossible d'ouvrir le fichier '%s'.\n", filename);
        exit(1);
    }

    // Lire l'en-tête PGM
    char magic[3];
    fscanf(file, "%s", magic);
    fclose(file);

    if (magic[0] == 'P' && magic[1] == '2') {
        image = readAsciiPGM(filename);
    }
    else if (magic[0] == 'P' && magic[1] == '5') {
        image = readBinaryPGM(filename);
    }
    else {
        fprintf(stderr, "%s n'est pas un fichier PGM valide.\n", filename);
        exit(1);
    }

    return image;
}


// Fonction pour lire un fichier PGM (ascii/P2) et générer la matrice de pixels
PGMImage readAsciiPGM(const char *filename) {
    PGMImage image;
    FILE *file = fopen(filename, "r");

    if (file == NULL) {
        fprintf(stderr, "Impossible d'ouvrir le fichier.\n");
        exit(1);
    }

    // Lire l'en-tête PGM
    char magic[3];
    fscanf(file, "%s", magic);
    if (magic[0] != 'P' || magic[1] != '2') {
        fprintf(stderr, "%s n'est pas un fichier PGM ascii valide.\n", filename);
        exit(1);
    }

    char debut[6], comment[200];
    fscanf(file, "%s", debut);
    // printf("Debut de ligne: %s\n", debut);

    fgetc(file);
    fscanf(file, "%[^\n]", comment);
    // printf("Commentaire: %s\n", comment);

    if(debut[0] == '#'){// Il y a un commentaire dans le fichier.
        fscanf(file, "%d %d", &image.width, &image.height);
    }
    else{ // Il n'y a pas de commentaires: c'est la largeur et la hauteur qu'on a lues
        image.width = atoi(debut);
        image.height = atoi(comment);
    }

    int maxVal;
    fscanf(file, "%d", &maxVal);

    // printf("Largeur=%d, Hauteur=%d, MaxVal=%d\n", image.width, image.height, maxVal);
    // exit(0);

    // Allouer de la mémoire pour la matrice de pixels
    image.pixels = (unsigned char **)malloc(image.height * sizeof(unsigned char *));    // car chaque ligne c'est un int* et on a (image.height) lignes
    for (int i = 0; i < image.height; ++i) {
        image.pixels[i] = (unsigned char *)malloc(image.width * sizeof(unsigned char));  // car chaque élément d'une ligne c'est un int
        for (int j = 0; j < image.width; ++j) {
            fscanf(file, "%d", &image.pixels[i][j]);
        }
    }
    return image;
}


// Fonction pour lire un fichier PGM (binaire/P5) et générer la matrice de pixels
PGMImage readBinaryPGM(const char *filename) {
    PGMImage image;
    FILE *file = fopen(filename, "rb");

    if (file == NULL) {
        fprintf(stderr, "Impossible d'ouvrir le fichier.\n");
        exit(1);
    }

    char magic[3];
    fscanf(file, "%s\n", magic);
    if (magic[0] != 'P' || magic[1] != '5') {
        fprintf(stderr, "%s n'est pas un fichier PGM binaire valide.\n", filename);
        exit(1);
    }

    fscanf(file, "%d %d\n", &image.width, &image.height);
    int maxVal;
    fscanf(file, "%d\n", &maxVal);

    image.pixels = (unsigned char **)malloc(image.height * sizeof(unsigned char *));
    for (int i = 0; i < image.height; ++i) {
        image.pixels[i] = (unsigned char *)malloc(image.width * sizeof(unsigned char));
        fread(image.pixels[i], sizeof(unsigned char), image.width, file);
    }

    fclose(file);
    return image;
}


// Fonction pour créer un fichier PGM (ascii/P2) à partir d'une matrice de pixels
void writeAsciiPGM(const char *filename, PGMImage image) {
    FILE *file = fopen(filename, "w");

    if (file == NULL) {
        fprintf(stderr, "Impossible de créer le fichier.\n");
        exit(1);
    }

    // Écrire l'en-tête PGM
    fprintf(file, "P2\n");
    fprintf(file, "%d %d\n", image.width, image.height);
    fprintf(file, "255\n"); // Valeur maximale, 255 pour une image en niveaux de gris

    // Écrire les pixels dans le fichier
    for (int i = 0; i < image.height; ++i) {
        for (int j = 0; j < image.width; ++j) {
            fprintf(file, "%d ", image.pixels[i][j]);
        }
        fprintf(file, "\n");
    }
}


// Fonction pour créer un fichier PGM (binaire/P5) à partir d'une matrice de pixels
void writeBinaryPGM(const char *filename, PGMImage image) {
    FILE *file = fopen(filename, "wb");

    if (file == NULL) {
        fprintf(stderr, "Impossible de créer le fichier.\n");
        exit(1);
    }

    fprintf(file, "P5\n");
    fprintf(file, "%d %d\n", image.width, image.height);
    fprintf(file, "255\n");

    for (int i = 0; i < image.height; ++i) {
        fwrite(image.pixels[i], sizeof(unsigned char), image.width, file);
    }

    fclose(file);
}


// Libère la mémoire occupée par l'image
void freeImage(PGMImage image){
    for (int i = 0; i < image.height; ++i) {
        free(image.pixels[i]);
    }
    free(image.pixels);
}


// Crée une image de même taille que celle passée en paramètre
// Si copy_pixels = true, copie aussi les pixels de l'image
PGMImage copyImage(PGMImage image, bool copy_pixels){
    PGMImage resultImage;

    resultImage.height = image.height;
    resultImage.width = image.width;

    // Allouer de la mémoire pour la matrice de pixels
    resultImage.pixels = (unsigned char **)malloc(image.height * sizeof(unsigned char *));
    for (int i = 0; i < image.height; ++i) {
        resultImage.pixels[i] = (unsigned char *)malloc(image.width * sizeof(unsigned char));

        if(copy_pixels){
            for (int j = 0; j < image.width; ++j) {
                resultImage.pixels[i][j] = image.pixels[i][j];
            }
        }
    }
    return resultImage;
}


// Crée une image de largeur 'width' et de hauteur 'height'
PGMImage createImage(int width, int height){
    if(width <= 0 || height <= 0){
        fprintf(stderr, "Erreur dans \"createImage\": les dimensions doivent être strictement positives!\n");
        exit(1);
    }
    PGMImage resultImage;

    resultImage.width = width;
    resultImage.height = height;

    // Allouer de la mémoire pour la matrice de pixels
    resultImage.pixels = (unsigned char **)malloc(height * sizeof(unsigned char *));
    for (int i = 0; i < height; ++i) {
        resultImage.pixels[i] = (unsigned char *)malloc(width * sizeof(unsigned char));
    }
    return resultImage;
}


// Crée une image de largeur 'width' et de hauteur 'height', avec tous les pixels de même valeur 'valeur_pixel'
PGMImage createImageWithColor(int width, int height, unsigned char valeur_pixel){

    PGMImage resultImage = createImage(width, height);
    for (int i = 0; i < height; ++i) {
        for (int j = 0; j < width; ++j) {
            resultImage.pixels[i][j] = valeur_pixel;
        }
    }
    return resultImage;
}


// Fonction principale pour tester les fonctions readPGM et writePGM
// int main() {
//     PGMImage image = readPGM("image.pgm");

//     // Traiter l'image ici...

//     writePGM("nouvelle_image.pgm", image);

//     // Libérer la mémoire
//     for (int i = 0; i < image.height; ++i) {
//         free(image.pixels[i]);
//     }
//     free(image.pixels);

//     return 0;
// }
