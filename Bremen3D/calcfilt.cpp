
#include <math.h>
#include <complex>
#include <QVector>
#include "defines.h"
#include "calcfilt.h"

std::complex<double> ejw[PLOTSIZE];
std::complex<double> jw;
std::complex<double> one(1,0);
std::complex<double> c;
QVector<double> f(PLOTSIZE);

QVector<double>* f_ref(){
    return &f;
}


void init_freqz(double fmin , double fmax)
{
    double S =log10(fmax) / log10(fmin);
    for (int i=0; i<PLOTSIZE; ++i){
        f[i] = FMIN *pow(10 ,S*(double) i / PLOTSIZE);
        jw.imag() = 2* M_PI * f[i] / FS;
        ejw[i]=exp(jw); // Precalc e^jw and where w = 2*pi f/fs
     }
}


void freqz(double B[3] , double A[2] , std::complex<double> H[PLOTSIZE]){
    for(int i=0 ; i<PLOTSIZE ; i++){
        c = ejw[i];
        H[i] =( B[0] + B[1]*c + B[2]*c*c )/ (one + A[0]*c + A[1]*c*c);
    }
}

void calcFilt(EQ_section_t &EQ , double fs, double Bcoef[3] , double Acoef[2] ){
double a0 ,a1 ,a2 ,b0,b1,b2;

 /*
%Calculate parametric EQ coef
    %f0 is the filter frequency
    %Q is the Q value
    %GaindB is the gain in dB
    %fs is the sampling frequency
    %Filtertype is one of
    % LPF (Lowpass Filter)
    % HPF (High pass filter)
    % BPFs (Band pass filter constant skirt gain, peak gain = Q)
    % BPFp (constant 0 dB peak gain)
    % notch
    % APF AllPass filter
    % peakingEQ
    % lowShelf
    % highSelf
   */
    double w0 = 2 * M_PI * EQ.Fc/fs;
    double alpha=sin(w0)/(2 * EQ.Q);
    double A = pow(10,EQ.Gain/40);
    double sqA = pow(10,EQ.Gain/20);
    double p; //prewarp factor
    switch(EQ.type){
    case LP1:
            p = tan(0.5*w0);
            a0 = 1;
            a1 = (p-1)/(p+1);
            a2 = 0;
            b0 = sqA*p/(p+1);
            b1 = b0;
            b2 = 0;
            break;
    case LP2 :
            b0 = sqA * (1 - cos(w0))/2;
            b1 =  2*b0;
            b2 =  b0;
            a0 =   1 + alpha;
            a1 =  -2*cos(w0);
            a2 =   1 - alpha;
           break;
    case HP1 :
            p = tan(0.5*w0);
            a0 = 1;
            a1 = (p-1)/(p+1);
            a2 = 0;
            b0 = sqA*(a0-a1)/2;
            b1 = -b0;
            b2 = 0;
            break;
    case HP2 :
            b0 = sqA* (1 + cos(w0))/2;
            b1 =  -2*b0;
            b2 =   b0;
            a0 =   1 + alpha;
            a1 =  -2*cos(w0);
            a2 =   1 - alpha;
            break;
            /*
    case 'BPFs' BandPass
            b0 =   sin(w0)/2 ;
            b1 =   0 ;
            b2 =  -b0;
            a0 =   1 + alpha;
            a1 =  -2*cos(w0);
            a2 =   1 - alpha;*/
    case BandPass : //'BPFp'

            b0 = sqA*  alpha;
            b1 =   0;
            b2 = sqA* -alpha;
            a0 =   1 + alpha;
            a1 =  -2*cos(w0);
            a2 =   1 - alpha;
            break;

     case Notch :
            b0 =   1;
            b1 =  -2*cos(w0);
            b2 =   1;
            a0 =   1 + alpha;
            a1 =  -2*cos(w0);
            a2 =   1 - alpha;
            break;
     case AllPass:
            b0 =   1 - alpha;
            b1 =  -2*cos(w0);
            b2 =   1 + alpha;
            a0 =   1 + alpha;
            a1 =  -2*cos(w0);
            a2 =   1 - alpha;
            break;
     case PeakingEQ:
            b0 =   1 + alpha*A;
            b1 =  -2*cos(w0);
            b2 =   1 - alpha*A;
            a0 =   1 + alpha/A;
            a1 =  -2*cos(w0);
            a2 =   1 - alpha/A;
            break;
      case LowShelf:
            b0 =    A*( (A+1) - (A-1)*cos(w0) + 2*sqrt(A)*alpha );
            b1 =  2*A*( (A-1) - (A+1)*cos(w0)                   );
            b2 =    A*( (A+1) - (A-1)*cos(w0) - 2*sqrt(A)*alpha );
            a0 =        (A+1) + (A-1)*cos(w0) + 2*sqrt(A)*alpha;
            a1 =   -2*( (A-1) + (A+1)*cos(w0)                   );
            a2 =        (A+1) + (A-1)*cos(w0) - 2*sqrt(A)*alpha;
            break;
      case HighShelf:
            b0 =    A*( (A+1) + (A-1)*cos(w0) + 2*sqrt(A)*alpha );
            b1 = -2*A*( (A-1) + (A+1)*cos(w0)                   );
            b2 =    A*( (A+1) + (A-1)*cos(w0) - 2*sqrt(A)*alpha );
            a0 =        (A+1) - (A-1)*cos(w0) + 2*sqrt(A)*alpha;
            a1 =    2*( (A-1) - (A+1)*cos(w0)                   );
            a2 =        (A+1) - (A-1)*cos(w0) - 2*sqrt(A)*alpha;
        break;}
    Acoef[0] = a1/a0;
    Acoef[1] = a2/a0;
    Bcoef[0] = b0/a0;
    Bcoef[1] = b1/a0;
    Bcoef[2] = b2/a0;
}



