#include "histogram.h"
#include <iostream>

Histogram::Histogram()
{
    maxValue = 0;
}

void Histogram::addValue(int x){
    data[x]++;
    if(data[x] > maxValue){
        maxValue = data[x];
    }
}

int Histogram::getMaxValue(){
    return maxValue;
}
