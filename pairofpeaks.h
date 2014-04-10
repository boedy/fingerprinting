#ifndef PAIROFPEAKS_H
#define PAIROFPEAKS_H

#include <vector>
#include <string>

using namespace std;

class PairOfPeaks
{
public:
    PairOfPeaks(pair<int,int>, pair<int,int>);
    pair<int,int> getFirstPeak();
    pair<int,int> getSecondPeak();
    int getAvgFreq();
    int getDeltaFreq();
    int getDeltaTime();
    string getHash();

private:
    pair< pair<int,int>, pair<int,int> > peaks;

};

#endif // PAIROFPEAKS_H
