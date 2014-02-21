#ifndef PEAKFINDER_H
#define PEAKFINDER_H

#include "ImageBuffer.h"
#include "ImageProcessor.h"

class PeakFinder : public ImageProcessor
{
public:
    PeakFinder(ImageBuffer *in);
    void process();
};

#endif // PEAKFINDER_H
