#include "basics.c"
#include "traitements_base.c"

// Ici j'effectue les tests de mes fonctions

int main() {
    
    PGMImage image1 = readPGM("./images/pgm binaire/goldhill.512.pgm");
    PGMImage image2 = readPGM("./images/pgm binaire/filtre_mixte.pgm");

    double L1 = luminance(image1), L2 = luminance(image2);
    double C1 = contraste(image1), C2 = contraste(image2);

    printf("Image 1: luminance = %lf, contraste = %lf\n", L1, C1);
    printf("Image 2: luminance = %lf, contraste = %lf\n", L2, C2);

    PGMImage image1_corrigee = redimensionner(image1, 1700, 1700);
    printf("Image 1 corrigee: luminance = %lf, contraste = %lf\n", luminance(image1_corrigee), contraste(image1_corrigee));

    writeBinaryPGM("./output/goldhill.1700.pgm", image1_corrigee);
    // writeAsciiPGM("./output/dwsample-pgm-1280.pgm", image1);

    // Libérer la mémoire
    freeImage(image1);
    freeImage(image2);
    freeImage(image1_corrigee);

    system("pause");

    return 0;
}









// int main() {
    
//     PGMImage image = readPGM("./images/pgm binaire/toys.1024.pgm");

//     // Traiter l'image ici...

//     writeAsciiPGM("./output/nouvelle_image_ascii.pgm", image);
//     writeBinaryPGM("./output/nouvelle_image_binaire.pgm", image);

//     // Libérer la mémoire
//     for (int i = 0; i < image.height; ++i) {
//         free(image.pixels[i]);
//     }
//     free(image.pixels);

//     system("pause");

//     return 0;
// }