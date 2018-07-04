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
void fftFrequencyDecimation(cdouble array[128], int power2size) {
    int n = 1 << power2size;
    int powerOf2, m , halfm, j, r;
    cdouble temp1, temp2;
    cdouble e;
    for (powerOf2 = power2size; powerOf2 >= 1; powerOf2--) {
        m = 1 << powerOf2;
        halfm = m >> 1;
        for (j = 0; j < halfm; j++) {
            e.re = cos(-2*M_PI*((double)j)/(double)m);
            e.im = sin(-2*M_PI*((double)j)/(double)m);
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
    fftFrequencyDecimation(dft, 7);
    bitReverse128(dft, 128);

}


double absComplex(cdouble num) {
    return sqrt(num.re*num.re + num.im*num.im);
}

int main(void)
{
    double x[128] = {1, 8.775826e-01, 5.403023e-01, 7.073720e-02, -4.161468e-01, -8.011436e-01, -9.899925e-01, -9.364567e-01, -6.536436e-01, -2.107958e-01, 2.836622e-01, 7.086698e-01, 9.601703e-01, 9.765876e-01, 7.539023e-01, 3.466353e-01, -1.455000e-01, -6.020119e-01, -9.111303e-01, -9.971722e-01, -8.390715e-01, -4.755369e-01, 4.425698e-03, 4.833048e-01, 8.438540e-01, 9.977983e-01, 9.074468e-01, 5.949207e-01, 1.367372e-01, -3.549243e-01, -7.596879e-01, -9.784535e-01, -9.576595e-01, -7.023971e-01, -2.751633e-01, 2.194400e-01, 6.603167e-01, 9.395249e-01, 9.887046e-01, 7.958150e-01, 4.080821e-01, -7.956357e-02, -5.477293e-01, -8.817917e-01, -9.999608e-01, -8.733046e-01, -5.328330e-01, -6.190529e-02, 4.241790e-01, 8.064095e-01, 9.912028e-01, 9.333151e-01, 6.469193e-01, 2.021351e-01, -2.921388e-01, -7.148870e-01, -9.626059e-01, -9.746453e-01, -7.480575e-01, -3.383192e-01, 1.542514e-01, 6.090560e-01, 9.147424e-01, 9.964679e-01, 8.342234e-01, 4.677318e-01, -1.327675e-02, -4.910347e-01, -8.485703e-01, -9.983462e-01, -9.036922e-01, -5.877828e-01, -1.279637e-01, 3.631854e-01, 7.654141e-01, 9.802426e-01, 9.550736e-01, 6.960693e-01, 2.666429e-01, -2.280669e-01, -6.669381e-01, -9.425195e-01, -9.873393e-01, -7.904240e-01, -3.999853e-01, 8.838370e-02, 5.551133e-01, 8.859318e-01, 9.998433e-01, 8.689583e-01, 5.253220e-01, 5.306854e-02, -4.321779e-01, -8.116122e-01, -9.923355e-01, -9.301004e-01, -6.401443e-01, -1.934586e-01, 3.005925e-01, 7.210482e-01, 9.649660e-01, 9.726266e-01, 7.421542e-01, 3.299766e-01, -1.629908e-01, -6.160523e-01, -9.182828e-01, -9.956856e-01, -8.293098e-01, -4.598901e-01, 2.212676e-02, 4.987262e-01, 8.532201e-01, 9.988160e-01, 8.998668e-01, 5.805989e-01, 1.191801e-01, -3.714181e-01, -7.710802e-01, -9.819550e-01, -9.524130e-01, -6.896870e-01, -2.581016e-01, 2.366760e-01, 6.735072e-01, 9.454402e-01, 9.858966e-01, 7.849711e-01};
    cdouble dftx[128];
    fftFrequencyDecimation128p(x, dftx, 128);
    printf("%f %f\n", dftx[0].re, dftx[1].re);
    printf("%f %f\n", dftx[0].im, dftx[1].im);
    return 0;
}
