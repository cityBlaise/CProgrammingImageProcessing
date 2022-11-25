#ifndef PROTOTYPE_H_INCLUDED
#define PROTOTYPE_H_INCLUDED

#include <string.h>

// enum for ppm format
typedef enum PPM_FORMAT { PPM_BINARY, PPM_PLAIN_TEXT } PPM_FORMAT;

// data type for color and rgb structure
typedef unsigned char pixval;
typedef struct _pixel {
    pixval r, g, b;
} pixel;

//get a caracter which is not within a comment line
char ppm_getc(FILE *fp);
int ppm_getint(FILE *fp);
PPM_FORMAT ppm_readmagicnumber(FILE *fp);
pixel *ppm_allocarray(int cols, int rows);
void ppm_readppminit(FILE *fp, int *colsP, int *rowsP, pixval *maxvalP, PPM_FORMAT *formatP);
void ppm_freearray(pixel *pixels);
pixel *ppm_readppm(FILE *fp, int *colsP, int *rowsP, pixval *maxvalP);
void ppm_writeppm(FILE *fred,FILE *fgreen,FILE *fblue, pixel *pixels, int cols, int rows, pixval maxvalP);
#endif // PROTOTYPE_H_INCLUDED
