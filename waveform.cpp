#include "waveform.h"
#include <stdlib.h>
#include <iostream>

using namespace std;

Waveform::Waveform(ImageBuffer *in, WavFile *wav) : ImageProcessor(in){
    wavFile = wav;
}

void Waveform::setWaveFile(WavFile *file){
    wavFile = file;
}

void Waveform::process(){
    int t;

    int peakMinify = 1000;
    int samplesToPixel = wavFile->getFrames()/640;
    //short* buffer = wavFile->getBuffer();

    short maxPeak = wavFile->getBuffer()[0];
    short minPeak = wavFile->getBuffer()[0];
    int i, k = 0, j = 0;
    for(i = 0; i < wavFile->getFrames(); i++){
        if(i%samplesToPixel != 0){
            if(maxPeak < wavFile->getBuffer()[i]){
                maxPeak = wavFile->getBuffer()[i];
            }else if(minPeak >= wavFile->getBuffer()[i]){
                minPeak = wavFile->getBuffer()[i];
            }
        }else{
            t = maxPeak/peakMinify;
            for(k = 0; k < abs(t); k++){
                //draw wavform
                In->setPixel(j,320-k, 0);
            }
            t = minPeak/peakMinify;
            for(k = 0; k < abs(t); k++){
                //draw wavform
                In->setPixel(j,320+k, 0);
            }

            maxPeak = wavFile->getBuffer()[i];
            minPeak = wavFile->getBuffer()[i];
            j++;
        }
    }
}

