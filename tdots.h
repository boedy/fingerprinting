#ifndef TDOTS_H
#define TDOTS_H

#include <stdint.h>
#include "Parameters.h"
#include "ImageBuffer.h"
#include "ImageProcessor.h"
#include <vector>

class TDots:public TImageProcessor{
  public:
    TDots(TImageBuffer *in, TImageBuffer *out);
    TDots(TImageBuffer *in, TImageBuffer *out, int tile_size, int);
    void process(void);
    void mark(int w, int h);
    void setPoint(int x, int y);
    void pairCalculation();
private:
    int tile_size;
    int filter_threshold;
    int dot_count;
    std::vector< std::pair<int,int> > points;
    std::vector< std::pair< std::pair<int,int>, std::pair<int,int> > > pairs;
};

#endif // TDOTS_H
