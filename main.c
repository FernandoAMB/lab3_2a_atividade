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

void fftFrequencyDecimation(cdouble *signal, cdouble * dft, int beginning, int size) {
    int halfSize = size >> 1;
    if (size >= 2) {
        int i = 0;
        for (i = beginning; i < beginning + halfSize; i++) {
            cdouble temp, temp2;
            temp.re = signal[i].re;
            temp.im = signal[i].im;
            temp2.re = signal[i + halfSize].re;
            temp2.im = signal[i + halfSize].im;
            dft[i].re = temp.re + temp2.re;
            dft[i].im = temp.im + temp2.im;
            dft[i + halfSize].re = (temp.re - temp2.re)*cos(2*M_PI*i/128);
            dft[i + halfSize].im = (temp.im - temp2.im)*sin(2*M_PI*i/128);
        }
    }
    if (size > 2) {
        fftFrequencyDecimation(dft, dft, beginning , halfSize);
        fftFrequencyDecimation(dft, dft, beginning + halfSize, halfSize);
    }
}

void bitReverse128(cdouble * array) {
    char bitRevOrder[] = {0, 64, 32, 96, 16, 80, 48, 112, 8, 72, 40, 104, 24, 88, 56, 120, 4, 68, 36, 100, 20, 84, 52, 116, 12, 76, 44, 108, 28, 92, 60, 124, 2, 66, 34, 98, 18, 82, 50, 114, 10, 74, 42, 106, 26, 90, 58, 122, 6, 70, 38, 102, 22, 86, 54, 118, 14, 78, 46, 110, 30, 94, 62, 126, 1, 65, 33, 97, 17, 81, 49, 113, 9, 73, 41, 105, 25, 89, 57, 121, 5, 69, 37, 101, 21, 85, 53, 117, 13, 77, 45, 109, 29, 93, 61, 125, 3, 67, 35, 99, 19, 83, 51, 115, 11, 75, 43, 107, 27, 91, 59, 123, 7, 71, 39, 103, 23, 87, 55, 119, 15, 79, 47, 111, 31, 95, 63, 127};
    int i;
    cdouble temp;
    for (i = 0; i < 128; i++) {
        if (bitRevOrder[i] != i) {
            int itemp;
            temp = array[i];
            array[i] = array[bitRevOrder[i]];
            array[bitRevOrder[i]] = temp;
            itemp = bitRevOrder[i];
            bitRevOrder[i] = i;
            bitRevOrder[itemp] = itemp;

        }
    }
}

void fftFrequencyDecimation128p(double *signal, cdouble * dft, int size) {
    int i;
    cdouble csignal[128];
    for (i = 0; i < 128; i++) {
        if (i < size) {
            csignal[i].re = signal[i];
            csignal[i].im = 0;
        }
        else {
            csignal[i].re = 0;
            csignal[i].im = 0;
        }
    }
    fftFrequencyDecimation(csignal, dft, 0, 128);
    bitReverse128(dft);

}


double absComplex(cdouble num) {
    return sqrt(num.re*num.re + num.im*num.im);
}

int main(void)
{
    double x[2] = {1 , 0};
    cdouble dftx[2];
    fftFrequencyDecimation128p(x, dftx, 2);
    printf("%f %f\n", dftx[0].re, dftx[1].re);
    printf("%f %f\n", dftx[0].im, dftx[1].im);
    return 0;
}
