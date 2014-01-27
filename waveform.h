#ifndef WAVEFORM_H
#define WAVEFORM_H

#include "ImageBuffer.h"
#include "ImageProcessor.h"
#include "wavfile.h"

class Waveform : public TImageProcessor
{
public:
    Waveform(TImageBuffer *in, WavFile * wav);
    void process();
    void setWaveFile(WavFile *);
private:
    WavFile * wavFile;
};

#endif // WAVEFORM_H
