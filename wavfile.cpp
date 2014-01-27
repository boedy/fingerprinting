#include "wavfile.h"
#include <iostream>
#include <stdlib.h>

using namespace std;

WavFile::WavFile(){
    filename = "hmpback1.wav";
}

WavFile::WavFile(char *f){
    filename = f;
}

WavFile::~WavFile(){
    free(obuf);
}

void WavFile::setFile(char *file){
    filename = file;
}

void WavFile::process(){

    mus_sound_initialize();	    /* initialize sndlib */
    int fd = mus_sound_open_input(filename); /* see if it exists */

    if (fd != -1)
    {
        cout << true;
        //close(fd);
        date = mus_sound_write_date(filename);
        srate = mus_sound_srate(filename);
        chans = mus_sound_chans(filename);
        samples = mus_sound_samples(filename);
        comment = mus_sound_comment(filename);
        length = (double)samples / (float)(chans * srate);
        frames = mus_sound_frames(filename);

        strftime(timestr, 64, "%a %d-%b-%y %H:%M %Z", localtime(&date));

        int i,j,k,n;
        int buffer_size = 1024;

        //allocate memory for buffer
        obuf = (short *)calloc(frames * chans, sizeof(short));

        //allocate memory for buffer
        bufs = (mus_float_t **)calloc(chans, sizeof(mus_float_t *));
        for (i=0;i<chans;i++)
            bufs[i] = (mus_float_t *)calloc(buffer_size, sizeof(mus_float_t));

        for (i = 0; i < frames; i += buffer_size)
        {
            mus_sound_read(fd, 0, buffer_size - 1, chans, bufs);
            for (k = 0, j = 0; k < buffer_size; k++, j += chans)
                for (n = 0; n < chans; n++){
                    obuf[j + i] = MUS_SAMPLE_TO_SHORT(bufs[n][k]);
                }
        }

        mus_sound_close_input(fd);

        for(int i = 0; i < chans; i++){
            free(bufs[i]);
        }
        free(bufs);
    }
}

short* WavFile::getBuffer(){
    return obuf;
}

float WavFile::getFrames(){
    return frames;
}

float WavFile::getRate(){
    return srate;
}

float WavFile::getLength(){
    return length;
}

int WavFile::getChannels(){
    return chans;
}

int WavFile::getSamples(){
    return samples;
}

short WavFile::getSample(int n){
    return obuf[n];
}

void WavFile::printInfo(){
    cout << "File: " << filename << endl;
    cout << "Rate: " << srate << endl;
    cout << "Channels: " << chans << endl;
    cout << "Frames: " << frames << endl;
    cout << "Length: " << length << endl;

    cout << "Type: " << mus_header_type_name(mus_sound_header_type(filename)) << endl;
    cout << "Format: " << mus_data_format_name(mus_sound_data_format(filename)) << endl;
    cout << "Written: " << timestr << endl;
}


