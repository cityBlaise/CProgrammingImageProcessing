#include <stdlib.h>
#include <stdio.h>
#include "prototype.h"

char ppm_getc(FILE *fp) {
    char ch = 0;

    // skip comments (start with #)
    while ((ch = getc(fp)) == '#') {
        while ((ch = getc(fp)) != '\n')
            ;
    }

    return ch;
}

int ppm_getint(FILE *fp) {
    char ch = 0;
    int i = 0;

    // keep reading a character until it is not whitespace
    // do while loop is suitable for checking ch after calling ppm_getc()
    // or this can be done by while(isspace(ch = ppm_getc(fp))) with ctype.h
    do {
        ch = ppm_getc(fp);
    } while (ch == ' ' || ch == '\t' || ch == '\n');

    // EOF check and number check
    if (ch == EOF) {
        fprintf(stderr, "Error: EOF detected during reading.\n");
        exit(1);
    }
    if (ch < '0' || ch > '9') {
        fprintf(stderr, "Error: Unknow data detected when parsing pixels.\n");
        exit(1);
    }

    // convert string to integer
    // above check ensure ch is a digit, so do first and check the next
    do {
        i = i * 10 + ch - '0';
        ch = ppm_getc(fp);
    } while (ch >= '0' && ch <= '9');

    return i;
}

PPM_FORMAT ppm_readmagicnumber(FILE *fp) {
    // check P
    if ((ppm_getc(fp)) != 'P') {
        fprintf(stderr, "Error: Incorrect format for PPM, the first line should be P3 or P6.\n");
        exit(1);
    }

    // check binary or plain text (3 or 6)
    int n = ppm_getint(fp);
    if (n == 3) {
        return PPM_PLAIN_TEXT;
    }
    if (n == 6) {
        return PPM_BINARY;
    }

    fprintf(stderr, "Error: Incorrect format for PPM, the first line should be P3 or P6.\n");
    exit(1);
}

pixel *ppm_allocarray(int cols, int rows) {
    pixel *pixels = NULL;

    pixels = (pixel *)malloc(rows * cols * sizeof(pixel));
    if (pixels == NULL) {
        fprintf(stderr, "Error: Memory allocation failed.\n");
        exit(1);
    }

    return pixels;
}

void ppm_readppminit(FILE *fp, int *colsP, int *rowsP, pixval *maxvalP, PPM_FORMAT *formatP) {
    *formatP = ppm_readmagicnumber(fp);
    if ((*colsP = ppm_getint(fp)) <= 0) {
        fprintf(stderr, "Error: Image width error, incorrect format for ppm.\n");
        exit(1);
    }
    if ((*rowsP = ppm_getint(fp)) <= 0) {
        fprintf(stderr, "Error: Image height error, incorrect format for ppm.\n");
        exit(1);
    }
    // read the max color value, must be 255 here
    if ((*maxvalP = (pixval)ppm_getint(fp)) != 255) {
        fprintf(stderr, "Error: Input PPM maximum colour value of %d found. Only 255 (8-bits per channel) is supported.\n", *maxvalP);
        exit(1);
    }
}

void ppm_freearray(pixel *pixels) {
    free(pixels);
}
pixel *ppm_readppm(FILE *fp, int *colsP, int *rowsP, pixval *maxvalP) {
    PPM_FORMAT format;
    pixel *pixels;

    ppm_readppminit(fp, colsP, rowsP, maxvalP, &format);
    pixels = ppm_allocarray(*colsP, *rowsP);

    int n_pixels = (*rowsP) * (*colsP);

    if (format == PPM_BINARY) {
        fread(pixels, sizeof(pixel), n_pixels, fp);
        return pixels;
    }
    if (format == PPM_PLAIN_TEXT) {
        int i;
        for ( i = 0; i < n_pixels; ++i) {
            pixels[i].r = ppm_getint(fp);
            pixels[i].g = ppm_getint(fp);
            pixels[i].b = ppm_getint(fp);
        }
        return pixels;
    }

    fprintf(stderr, "Error: Incorrect format for PPM, the first line should be P3 or P6.\n");
    exit(1);
}

void ppm_writeppm(FILE *fred,FILE *fgreen,FILE *fblue, pixel *pixels, int cols, int rows, pixval maxvalP) {

    fprintf(fred, "P3\n%d\n%d\n%d", cols, rows, maxvalP);
    fprintf(fgreen, "P3\n%d\n%d\n%d", cols, rows, maxvalP);
    fprintf(fblue, "P3\n%d\n%d\n%d", cols, rows, maxvalP);

    int i;
    for (i = 0; i < rows; ++i) {
        int index = i * cols;
        fprintf(fred, "\n");
        fprintf(fgreen, "\n");
        fprintf(fblue, "\n");
        int j ;
        for (j = 0; j < cols; ++j) {
            fprintf(fred, "%d %d %d\t", pixels[index].r, 0, 0);
            fprintf(fgreen, "%d %d %d\t", 0, pixels[index].g, 0);
            fprintf(fblue, "%d %d %d\t", 0, 0, pixels[index].b);
            ++index;
        }
    }
}
