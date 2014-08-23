#ifndef CALCFILT_H
#define CALCFILT_H

#include <QVector>
#include <complex>
#include "defines.h"


enum FilterType{LowShelf , HighSelf , PeakingEQ , Notch , AllPass ,LowPass1, LowPass, HighPass1, HighPass , BandPass};

QVector<double>* f_ref();
void calcFilt(double f0, double Q, double GaindB , double fs, FilterType type , double Bcoef[3] , double Acoef[2] );
void init_freqz(double fmin , double fmax );
void freqz(double B[3] , double A[2] , std::complex<double> H[]);

#endif // CALCFILT_H
