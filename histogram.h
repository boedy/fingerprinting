#ifndef HISTOGRAM_H
#define HISTOGRAM_H

#include <map>

using namespace std;

class Histogram
{
public:
    Histogram();
    void addValue(int);
    int getMaxValue();
private:
    map<int, unsigned int> data;
    int maxValue;
};

#endif // HISTOGRAM_H
