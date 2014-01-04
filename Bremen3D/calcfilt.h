#ifndef CALCFILT_H
#define CALCFILT_H

#include <QVector>
#include <complex>

enum FilterType{LowShelf , HighSelf , PeakingEQ , Notch , AllPass , HighPass , LowPass , BandPass};

void calcFilt(double f0, double Q, double GaindB , double fs, FilterType type , double Bcoef[3] , double Acoef[2] );

void freqz(double B[3] , double A[2] , double fs , QVector<double> *f , std::complex<double> H[] ,std::complex<double> ejw[] );

#endif // CALCFILT_H
