#ifndef TDOTS_H
#define TDOTS_H

#include <stdint.h>
#include "Parameters.h"
#include "ImageBuffer.h"
#include "ImageProcessor.h"
#include <vector>
#include <set>
#include <string>

using namespace std;

class TDots:public ImageProcessor{
  public:
    TDots(ImageBuffer *in);
    TDots(ImageBuffer *in, int tile_size, int);
    void process(void);
    void mark(int w, int h);
    void setPoint(int x, int y);
    void pairCalculation();
    void symmertyDetection();
private:
    int tile_size;
    int tile_height;
    int tile_width;
    int filter_threshold;
    int dot_count;
    vector< pair<int,int> > points;
    vector< pair< pair<int,int>, pair<int,int> > > pairs;
//    set< pair< pair<int,int>, pair<int,int> > > pairs;
    set< string > hashes;
};

#endif // TDOTS_H
