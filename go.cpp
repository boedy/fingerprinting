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
#include "peakfinder.h"
#include "smooth.h"
#include "wavfile.h"
#include "waveform.h"
#include "spectrogram.h"
#include "database.h"
#include "hashgenerator.h"
#include "pairofpeaks.h"

using namespace std;

char* toCharPointer(string image_file){
    char * image_file_char;

    image_file_char = new char[image_file.size() + 1];
    copy(image_file.begin(), image_file.end(), image_file_char);
    image_file_char[image_file.size()] = '\0'; // don't forget the terminating 0
    return image_file_char;
}

vector<string> searchInFolder(string folder){
    DIR* dirp = opendir(folder.c_str());
    if(dirp != NULL){
        struct dirent* dp;
        vector<string> files;
        while ((dp = readdir(dirp)) != NULL) {
            string filename = dp->d_name;
            if (filename.substr(filename.find_last_of(".")) == ".wav"){
                files.push_back(filename);
            }
        }
        return files;
    }
}

void runAlgorithm(string folder, string type){
    //get all files in folder
    vector<string> files = searchInFolder(folder);
    string dest_image_folder = "image_files/";

    for(int i = 0; i < files.size(); i++){

        WavFile *wav = new WavFile(toCharPointer(folder+files[i]));
        wav->process();

        ImageBuffer *buffer1 = new ImageBuffer((int)wav->getSamples()/256,256);
        buffer1->set();

        string imageFile = files[i];
        imageFile.replace(imageFile.end()-3, imageFile.end(), "pgm");
        ImageFile *file = new ImageFile(toCharPointer(dest_image_folder+imageFile), buffer1);

        Spectrogram *spec = new Spectrogram(buffer1, wav);
        spec->process();

        PeakFinder *pf = new PeakFinder(buffer1, 32, 180);
        pf->process();
        pf->combinePeaks();
        vector< PairOfPeaks*> peaks = pf->getPairOfPeaks();
        if(type == "store"){
            for(vector< PairOfPeaks*>::iterator it = peaks.begin(); it != peaks.end(); it++){
                cout << (*it)->getHash() << endl;
            }
        }

//        file->write();
        delete wav;
//        delete file;
        delete buffer1;
//        delete spec;
//        delete pf;

    }
}

int main(){
    string audio_reference_folder = "audio_files/wav/";
    string audio_sample_folder = "audio_files/samples/";

//    Database<string>* db1 = new Database<string>("int");
//    Database<int>* db2 = new Database<int>("string");

//    db1->insert("foo", "bar");
//    db2->insert("foo", 1);

//    cout << db1->get("foo") << endl;
//    cout << db2->get("food") << endl;


    runAlgorithm(audio_sample_folder, "store");

    return 0;
}


