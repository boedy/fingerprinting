#ifndef SPECTROGRAM_H
#define SPECTROGRAM_H

#include "ImageBuffer.h"
#include "ImageProcessor.h"
#include "wavfile.h"
#include <fftw3.h>

class Spectrogram : public TImageProcessor
{
public:
    Spectrogram(TImageBuffer *in, WavFile * wav);
    void process();
    void setWaveFile(WavFile *);
    double windowBlackmanHarris(double n, double N);
    double windowHanning(double n, double N);
private:
    WavFile * wavFile;
};

#endif // SPECTROGRAM_H
