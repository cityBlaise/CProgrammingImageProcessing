#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "prototype.h"

int main(int argc, char *argv[])
{
     FILE* fichier = NULL;
     fichier = fopen(argv[1],"r");
     if(fichier){
        printf("%s ouverture okay",argv[1]);
        int cols, rows;
        pixval maxvalP;
        pixel *pixels = ppm_readppm(fichier, &cols, &rows, &maxvalP);

        FILE* fichier1 =NULL;
        FILE* fichier2 =NULL;
        FILE* fichier3=NULL;
         fichier1= fopen("red.ppm","w");
         fichier2 = fopen("green.ppm","w");
         fichier3 = fopen("blue.ppm","w");
        if(fichier1 && fichier2 && fichier3 ){
            ppm_writeppm(fichier1,fichier2,fichier3, pixels, cols, rows, maxvalP);
            fclose(fichier1);
            fclose(fichier2);
            fclose(fichier2);
        }else{
            fprintf(stderr, "Error: Memory allocation failed.\n");
        }


        printf("\nfin");

        fclose(fichier);
     }else{
         printf("ouverture not okay");
        fclose(fichier);
     }
     return 0;
}
