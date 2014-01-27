#ifndef TDOTS_H
#define TDOTS_H

#include <stdint.h>
#include "Parameters.h"
#include "ImageBuffer.h"
#include "ImageProcessor.h"

class TDots:public TImageProcessor{
  public:
    TDots(TImageBuffer *in, TImageBuffer *out);
    TDots(TImageBuffer *in, TImageBuffer *out, int tile_size);
    void process(void);
    void mark(int w, int h);
private:
    int tile_size;
    int filter_threshold;
};

#endif // TDOTS_H
