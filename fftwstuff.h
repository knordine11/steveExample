#ifndef FFTWSTUFF_H
#define FFTWSTUFF_H

#include <QObject>
#include "fftw3/fftw3.h"
#include <cmath>

extern float rec_arr[500000];
extern int rec_arr_cnt;

class fftwStuff : public QObject
{
    Q_OBJECT
public:
    explicit fftwStuff(QObject *parent = nullptr);
    static void DoIt(int frame_start, int length);
    static double abs(fftw_complex);
    static double bin_freq(size_t, size_t, double);

signals:
};

#endif // FFTWSTUFF_H
