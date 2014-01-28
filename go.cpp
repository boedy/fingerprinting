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
#include "Parameters.h"
#include "ImageBuffer.h" 
#include "ImageFile.h"
#include "ImageProcessor.h" 
#include "tdots.h"
#include "wavfile.h"
#include "waveform.h"
#include "spectrogram.h"


using namespace std;

char* getImage(string image_file){
    char * image_file_char;

    image_file_char = new char[image_file.size() + 1];
    copy(image_file.begin(), image_file.end(), image_file_char);
    image_file_char[image_file.size()] = '\0'; // don't forget the terminating 0
    return image_file_char;
}

int main(){

    TImageBuffer *Buffer1;
    TImageFile *File1;

        WavFile *wav = new WavFile("crystalised.wav");
//    WavFile *wav = new WavFile("hmpback1.wav");

    wav->process();
    wav->printInfo();

    Buffer1 = new TImageBuffer((int)wav->getSamples()/512,512);
    Buffer1->set();

    File1 = new TImageFile("spectrogram.pgm", Buffer1);

//    Waveform *waveform = new Waveform(Buffer1, wav);
//    waveform->process();

    Spectrogram *spec = new Spectrogram(Buffer1, wav);
    spec->process();

    TDots *dots = new TDots(Buffer1, Buffer1, 32, 240);
    dots->process();

    File1->write();

    return 0;
}

//int main(int argc, char **argv){
//    TImageBuffer *Buffer1, *Buffer2;
//    TImageFile *File1, *File2;
//    TDots *dots;

//    string bash_script = "./generate_spectrogram.sh ";
//    string files[3], bash_command, image_file;

//    //de bestanden: alleen wav bestanden in de map sounds_src
//    files[0] = "piano";
//    files[1] = "piano_pitch";
//    files[2] = "piano_amp";

//    int file_array_size = (sizeof(files)/sizeof(*files));

//    Buffer1 = new TImageBuffer();
//    Buffer2 = new TImageBuffer();
//    dots = new TDots(Buffer1, Buffer2, 64);

//    for(int i=0; i < file_array_size; i++){
//        bash_command = "";
//        bash_command.append(bash_script).append("sounds_src/").append(files[i]).append(".wav");
//        cout << bash_command << endl;

//        printf("Processing ...\n");
//        //generate spectrograms from audio files
////        system(bash_command.c_str());

//        image_file = "images/";
//        image_file.append(files[i]).append(".pgm");

//        File1 = new TImageFile(getImage(image_file), Buffer1);

//        image_file = "images_complete/";
//        image_file.append(files[i]).append(".pgm");

//        File2 = new TImageFile(getImage(image_file), Buffer2);

//        File1->read();

//        //Doe hier de analyse
//        dots->process();

//        File2->write();
//    }

//    return 0;
//}


