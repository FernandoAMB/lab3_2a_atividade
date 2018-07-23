#include <stdio.h>
#include <stdlib.h>
#include "tistdtypes.h"
#include "Dsplib.h"
#include "overlapSaveFiltering.h"

/**
 * main.c
 */

#define saturate(X) ((Int16)((X > 0x7FFF) ? X = 0x7FFF : ((X < (Int16)-0x8000) ? X = -0x8000: X)))


typedef struct {
    Int16 re, im;
}cfixed;

DATA prodCompReFixed(cfixed a, cfixed b) {
    LDATA num1[2], num2[2];
    num1[0] = (LDATA) a.re;
    num1[1] = (LDATA) a.im;
    num2[0] = (LDATA) b.re;
    num2[1] = (LDATA) b.im;
    LDATA result[2];
    DATA res[2];
    mul32(num1, num2, result, 2);
    result[0] >>= 15;
    result[1] >>= 15;
    res[0] = (DATA) result[0];
    res[1] = (DATA) result[1];

    sub(&res[0], &res[1], res, 1, 0);

    return res[0];
}

Int16 prodCompImFixed(cfixed a, cfixed b) {
        LDATA num1[2], num2[2];
        num1[0] = (LDATA) a.re;
        num1[1] = (LDATA) a.im;
        num2[0] = (LDATA) b.im;
        num2[1] = (LDATA) b.re;
        LDATA result[2];
            DATA res[2];
            mul32(num1, num2, result, 2);
            result[0] >>= 15;
            result[1] >>= 15;
            res[0] = (DATA) result[0];
            res[1] = (DATA) result[1];

            add(&res[0], &res[1], res, 1, 0);

            return res[0];
}

Int16 sum(DATA a, DATA b) {
    DATA result;
    add(&a , &b, &result, 1, 0);
    return result;
}



void bitReverse128Fixed(cfixed * array, int size) {
    if (size <= 2) return;
    int r = 0;
    int i = 0;
    cfixed temp;
    for (i = 1; i < size ; i++) {
        r = getNextRevBit(r, size);
        if (r > i) {
            temp = array[i];
            array[i] = array[r];
            array[r] = temp;
        }
    }
}



void fftFrequencyDecimationFixed(cfixed * array, int powerOf2Size) {
    int n = 1 << powerOf2Size;
    int powerOf2, m , halfm, j, r;
    cfixed temp1, temp2;
    cfixed e;
    DATA angleSine, angleCos;
    cfixed *q;
    for (powerOf2 = powerOf2Size; powerOf2 >= 1; powerOf2--) {
            m = 1 << powerOf2;
            halfm = m >> 1;
            for (j = 0; j < halfm; j++) {
                angleSine = ((Int16)(((double)j)/m)*0x7FFF) << 1; //tenho que descobrir um jeito de fazer isso com ponto flutuante
                if (angleSine > 0x4000) {
                    angleCos = 0x7FFF - (angleSine - 0x4000);
                }
                else {
                    angleCos = angleSine + 0x4000;
                }
                sine(&angleCos, &(q->re), 1);
                sine(&angleSine, &(q->im), 1);
                e.re = q->re;
                e.im = q->im;
                for (r = 0; r < n; r += m) {
                    temp1 = array[r+j];
                    temp2 = array[r+j+halfm];
                    array[r + j].re = sum(temp1.re, temp2.re);
                    array[r + j].im = sum(temp1.im, temp2.im);
                    array[r + j + halfm].re = sum(temp1.re, -temp2.re);
                    array[r + j + halfm].im = sum(temp1.re, -temp2.re);
                    temp1.re = prodCompReFixed(array[r + j + halfm], e);
                    temp1.im = prodCompImFixed(array[r + j + halfm], e);
                    array[r+j+halfm] = temp1;

                }
            }

        }
}

void fftFixedPoint(Int16 * signal, cfixed * dft, int size) {
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
    fftFrequencyDecimationFixed(dft, 7);
    bitReverse128Fixed(dft, 128);


}

int main(void)
{
//    cfixed dfixed[128];
//    Int16 xf[128];
//    int i;
//    for (i = 0; i < 128; i++) {
//        xf[i] = (Int16)(x[i] * 0x7FFF);
//    }
    double result[1024];
    int i = 0;
    for (i = 0; i < 1024; i++) {
        result[i] = 0;
    }
    fastConvOverlapSave(filter, signal, result, signalSize);
//    fftFixedPoint(xf, dfixed, 128);
    printf("gg\n");
    return 0;
}
