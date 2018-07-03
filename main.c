#include <math.h>
#include <stdio.h>
#include <stdlib.h>

/**
 * main.c
 */

#define M_PI        3.14159265358979323846  /* pi */
#define prodcompRe(X, Y) ((X).re*(Y).re - (X).im*(Y).im)
#define prodcompIm(X, Y) ((X).im*(Y).re + (X).re*(Y).im)


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


//peguei essa bruxaria de um alemão aí (jjj.de), tenho que verificar depois
inline unsigned getNextRevBit(unsigned r, unsigned n) {
    unsigned m;
    for (m = n >> 1; (!((r^=m)&m)); m >>=1);
    return r;
}

void hardCopycdouble(cdouble * source, cdouble * dest) {
    dest->re = source->re;
    dest->im = source->im;
}

void bitReverse128(cdouble * array, int size) {
    if (size <= 2) return;
    int r = 0;
    int i = 0;
    cdouble temp;
    for (i = 1; i < size ; i++) {
        r = getNextRevBit(r, size);
        if (r > i) {
            temp = array[i];
            array[i] = array[r];
            array[r] = temp;
        }
    }
}

/*
 * power2size = 7 pra 128 pontos (2^7 = 128)
 */
void fftFrequencyDecimation(cdouble *array, int power2size) {
    int n = 1 << power2size;
    int powerOf2, m , halfm, j, r;
    cdouble temp1, temp2;
    cdouble e;
    for (powerOf2 = power2size; powerOf2 >= 1; powerOf2--) {
        m = 1 << powerOf2;
        halfm = m >> 1;
        for (j = 0; j < halfm; j++) {
            e.re = cos(2*M_PI*((double)j)/(double)m);
            e.im = sin(2*M_PI*j/m);
            for (r = 0; r < n; r += m) {
                hardCopycdouble(&array[r + j], &temp1);
                hardCopycdouble(&array[r +j + halfm], &temp2);
                array[r + j].re = temp1.re + temp2.re;
                array[r + j].im = temp1.im + temp2.im;
                array[r + j + halfm].re = temp1.re - temp2.re;
                array[r + j + halfm].im = temp1.im - temp2.im;
                array[r + j + halfm].re = prodcompRe(array[r + j + halfm], e);
                array[r + j + halfm].im = prodcompIm(array[r + j + halfm], e);

            }
        }

    }
}



void fftFrequencyDecimation128p(double *signal, cdouble * dft, int size) {
    int i;
    for (i = 0; i < 128; i++) {
        if (i < size) {
            dft[i].re = signal[i];
            dft[i].im = 0;
        }
        else {
            dft[i].re = 0;
            dft[i].im = 0;
        }
    }
    fftFrequencyDecimation(dft, 7);
    bitReverse128(dft, 128);

}


double absComplex(cdouble num) {
    return sqrt(num.re*num.re + num.im*num.im);
}

int main(void)
{
    double x[128];
    x[0] = 1;
    int i = 1;
    for (i = 1; i < 128;i++) x[i] = 0;
    cdouble dftx[128];
    fftFrequencyDecimation128p(x, dftx, 2);
    printf("%f %f\n", dftx[0].re, dftx[1].re);
    printf("%f %f\n", dftx[0].im, dftx[1].im);
    return 0;
}
