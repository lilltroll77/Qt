#ifndef CALCFILT_H
#define CALCFILT_H

#include <QVector>
#include <complex>
#include "defines.h"
enum filterType_t{LP1, LP2 , HP1 , HP2 , LowShelf , HighShelf , PeakingEQ , Notch , AllPass , BandPass , Mute};

//Must match XMOS code

typedef struct{
        int active;
        enum filterType_t type;
        float Fc;
        int link;
        float Q;
        float Gain;
        float MasterGain;
    }EQ_section_t;

typedef struct{
    EQ_section_t section[8];
    unsigned delay;
}EQ_channel_t;

QVector<double>* f_ref();
void calcFilt(EQ_section_t &EQ , double fs, double Bcoef[3] , double Acoef[2] );
void init_freqz(double fmin , double fmax );
void freqz(double B[3] , double A[2] , std::complex<double> H[]);

#endif // CALCFILT_H
