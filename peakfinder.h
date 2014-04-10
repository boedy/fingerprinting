#ifndef PEAKFINDER_H
#define PEAKFINDER_H

#include "ImageBuffer.h"
#include "ImageProcessor.h"

#include <stdint.h>
#include "Parameters.h"
#include "ImageBuffer.h"
#include "ImageProcessor.h"
#include "pairofpeaks.h"
#include <vector>
#include <set>
#include <string>

using namespace std;

class PeakFinder:public ImageProcessor{
  public:
    PeakFinder(ImageBuffer *in);
    PeakFinder(ImageBuffer *in, int tile_size, int);
    void process(void);
    void mark(int w, int h);
    void addPeak(int x, int y);
    void combinePeaks();
    vector< PairOfPeaks*> getPairOfPeaks();

private:
    int tile_size;
    int tile_height;
    int tile_width;
    int filter_threshold;
    int peak_count;
    vector< PairOfPeaks*> pairsOfPeaks;
    vector< pair<int,int> > peaks;

    set< string > hashes;
};

#endif // PEAKFINDER_H
