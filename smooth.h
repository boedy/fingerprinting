#ifndef SMOOTH_H
#define SMOOTH_H

#include "ImageBuffer.h"
#include "ImageProcessor.h"

class Smooth : public ImageProcessor
{
public:
    Smooth(ImageBuffer *in, int);
    void process();
private:
    int size;
};

#endif // SMOOTH_H
