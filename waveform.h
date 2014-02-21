#ifndef WAVEFORM_H
#define WAVEFORM_H

#include "ImageBuffer.h"
#include "ImageProcessor.h"
#include "wavfile.h"

class Waveform : public ImageProcessor
{
public:
    Waveform(ImageBuffer *in, WavFile * wav);
    void process();
    void setWaveFile(WavFile *);
private:
    WavFile * wavFile;
};

#endif // WAVEFORM_H
