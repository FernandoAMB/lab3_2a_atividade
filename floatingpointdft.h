#include <math.h>

/*
 * floatingpointdft.h
 *
 *  Created on: 17 de jul de 2018
 *      Author: ferna
 */

#ifndef FLOATINGPOINTDFT_H_
#define FLOATINGPOINTDFT_H_


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
 * revBit = 1 para fft
 * revBit = -1 para ifft
 */
void fftFrequencyDecimation(cdouble array[128], int power2size, int revBit) {
    int n = 1 << power2size;
    int powerOf2, m , halfm, j, r;
    cdouble temp1, temp2;
    cdouble e;
    for (powerOf2 = power2size; powerOf2 >= 1; powerOf2--) {
        m = 1 << powerOf2;
        halfm = m >> 1;
        for (j = 0; j < halfm; j++) {
            e.re = cos(-revBit*2*M_PI*((double)j)/(double)m);
            e.im = sin(-revBit*2*M_PI*((double)j)/(double)m);
            for (r = 0; r < n; r += m) {
                hardCopycdouble(&array[r + j], &temp1);
                hardCopycdouble(&array[r +j + halfm], &temp2);
                array[r + j].re = temp1.re + temp2.re;
                array[r + j].im = temp1.im + temp2.im;
                array[r + j + halfm].re = temp1.re - temp2.re;
                array[r + j + halfm].im = temp1.im - temp2.im;
                temp1.re = prodcompRe(array[r + j + halfm], e);
                temp1.im = prodcompIm(array[r + j + halfm], e);
                hardCopycdouble(&temp1, &array[r + j + halfm]);

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
    fftFrequencyDecimation(dft, 7, 1);
    bitReverse128(dft, 128);

}

void ifftFrequencyDecimation128p(cdouble * dft, double * result) {
    fftFrequencyDecimation(dft, 7, -1);
    bitReverse128(dft, 128);
    int i;
    for (i = 0; i < 128; i++) {
        if (dft[i].im != 0) {
            printf("Erro na ifft, indice %d", i);
        }
        else {
            result[i] = dft[i].re;
        }
    }
}


double absComplex(cdouble num) {
    return sqrt(num.re*num.re + num.im*num.im);
}


#endif /* FLOATINGPOINTDFT_H_ */
