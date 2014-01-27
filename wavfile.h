#ifndef WAVFILE_H
#define WAVFILE_H
#define MUS_SAMPLE_TO_SHORT(n) ((short)((n) * (1 << 15)))

#include "ImageBuffer.h"
#include "ImageProcessor.h"
#include <sndlib.h>

class WavFile{
public:
    WavFile();
    WavFile(char* filename);
    ~WavFile();
    void setFile(char *);
    void process();
    void printInfo();
    short* getBuffer();
    float getFrames();
    float getLength();
    float getRate();
    int getSamples();
    short getSample(int n);
    int getChannels();
private:
    char* filename;
    int chans, srate;
    mus_long_t samples, frames;
    mus_float_t **bufs;
    short *obuf;
    float length;
    time_t date;
    char *comment;
    char timestr[64];
};

#endif // WAVFILE_H
