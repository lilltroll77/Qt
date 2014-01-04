#include "transform.h"


Transform::Transform()
{
}


//Translates from log scale to linscale
int Transform::log2lin(double arg, int max, double maxLog, double minLog){
    double value = log10(arg/minLog)/log10(maxLog/minLog)*(double)(max);
    return (int)round(value);
}

double Transform::lin2log(int val,int max, double maxLog, double minLog){
    //y=0;min*10^(y/99*log10(max/min))
    double scale = log10(maxLog/minLog)/max;
    return minLog*pow(10, (double)val * scale);
}
