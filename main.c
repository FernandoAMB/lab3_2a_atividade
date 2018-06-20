#include <math.h>
#include <stdio.h>

/**
 * main.c
 */

#define M_PI        3.14159265358979323846  /* pi */


typedef struct {
    double re, im;
}cdouble;

void dft(double *signal, cdouble * dft, int size) {
    int i, j;
    for (i = 0; i<size; i++) {
        dft[i].re = 0;
        dft[i].im = 0;
        for (j = 0; j<size; j++) {
            dft[i].re += signal[j]*cos(2*M_PI*i*j/size);
            dft[i].im += -1*signal[j]*sin(2*M_PI*i*j/size);
        }
    }
}

double absComplex(cdouble num) {
    return sqrt(num.re*num.re + num.im*num.im);
}

int main(void)
{
    double x[2] = {1 , 0};
    cdouble dftx[2];
    dft(x, dftx, 2);
    printf("%f %f\n", dftx[0].re, dftx[1].re);
    printf("%f %f\n", dftx[0].im, dftx[1].im);
    return 0;
}
