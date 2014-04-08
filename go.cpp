/* go.cpp v1.0: Demonstration of the TIVision classes.
 * Copyright 2008 Pieter Burghouwt Haagse Hogeschool Rijswijk
 *
 * Application will mirror picture image1.pgm in the vertical axis
 * Result is shown and written to image2.pgm.
 * As an alternative source the camera (Allied Guppy 33B) can be
 * used by uncommenting the camera-statements and commenting
 * the read from image1.
 *
 * This file is part of TIVision.
 * TIVision is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * TIVision is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * You should have received a copy of the GNU General Public License
 * along with TIVision.  If not, see <http://www.gnu.org/licenses/>.
 */
#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <array>
#include <dirent.h>
#include <string.h>
#include "Parameters.h"
#include "ImageBuffer.h" 
#include "ImageFile.h"
#include "ImageProcessor.h" 
#include "tdots.h"
#include "peakfinder.h"
#include "smooth.h"
#include "wavfile.h"
#include "waveform.h"
#include "spectrogram.h"


using namespace std;

char* toCharPointer(string image_file){
    char * image_file_char;

    image_file_char = new char[image_file.size() + 1];
    copy(image_file.begin(), image_file.end(), image_file_char);
    image_file_char[image_file.size()] = '\0'; // don't forget the terminating 0
    return image_file_char;
}

vector<string> searchInFolder(char * folder){
    DIR* dirp = opendir(folder);
    if(dirp != NULL){
        struct dirent* dp;
        vector<string> files;
        while ((dp = readdir(dirp)) != NULL) {
            if (strstr(dp->d_name, ".wav") != 0) {
                string total = string(folder)+ "/" + string(dp->d_name);
                files.push_back(string(folder)+ "/" + string(dp->d_name));

                cout << total << endl;
            }
        }
        return files;
    }
}

int main(){

    vector<string> files;
    string temp;

    //    filename[0] = "music/samples/downsampled";
    //    filename[1] = "music/samples/downsampled_uppitch_10";


    files = searchInFolder("audio_files/samples");

//    files[0] = "music/samples/bbq-bob";
//    files[1] = "music/samples/bbq-bob_uppitch_10";

//    for(int i = 0; i < files.size(); i++){
//        temp = files[i];
//        WavFile *wav = new WavFile(toCharPointer(temp.append(".wav")));
//        wav->process();
//        //            wav->printInfo();

//        ImageBuffer *buffer1 = new ImageBuffer((int)wav->getSamples()/256,256);
//        buffer1->set();

//        temp = files[i];
//        ImageFile *File1 = new ImageFile(toCharPointer(temp.append(".pgm")), buffer1);

//        Spectrogram *spec = new Spectrogram(buffer1, wav);
//        spec->process();

//        TDots *dots = new TDots(buffer1, 32, 180);
//        dots->process();

//        File1->write();
//        delete wav;
//        delete spec;
//        delete buffer1;
//        delete dots;

//    }
    return 0;
}


