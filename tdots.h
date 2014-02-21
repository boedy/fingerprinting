#ifndef TDOTS_H
#define TDOTS_H

#include <stdint.h>
#include "Parameters.h"
#include "ImageBuffer.h"
#include "ImageProcessor.h"
#include <vector>
#include <set>
#include <string>

class TDots:public ImageProcessor{
  public:
    TDots(ImageBuffer *in, ImageBuffer *out);
    TDots(ImageBuffer *in, ImageBuffer *out, int tile_size, int);
    void process(void);
    void mark(int w, int h);
    void setPoint(int x, int y);
    void pairCalculation();
    void symmertyDetection();
private:
    int tile_size;
    int filter_threshold;
    int dot_count;
    std::vector< std::pair<int,int> > points;
    std::vector< std::pair< std::pair<int,int>, std::pair<int,int> > > pairs;
//    std::set< std::pair< std::pair<int,int>, std::pair<int,int> > > pairs;
    std::set< std::string > hashes;
};

#endif // TDOTS_H
