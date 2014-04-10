#include "pairofpeaks.h"
#include <cmath>

PairOfPeaks::PairOfPeaks(pair<int,int> peak1, pair<int,int> peak2)
{
    peaks = make_pair(peak1, peak2);
}

pair<int,int> PairOfPeaks::getFirstPeak(){
    return peaks.first;
}

pair<int,int> PairOfPeaks::getSecondPeak(){
    return peaks.second;
}

int PairOfPeaks::getAvgFreq(){
    return peaks.first.second / 10;
}

int PairOfPeaks::getDeltaFreq(){
    return abs(peaks.first.second - peaks.second.second);
}

int PairOfPeaks::getDeltaTime(){
    return abs(peaks.first.first - peaks.second.first);
}

string PairOfPeaks::getHash(){
    string hash = "";
    hash.append(to_string(getAvgFreq())).append("|").append(to_string(getDeltaFreq())).append("|").append(to_string(getDeltaTime()));
    return hash;
}
