#include "peakfinder.h"
#include <iostream>
#include <cmath>
#include <set>

using namespace std;



PeakFinder::PeakFinder(ImageBuffer *in) : ImageProcessor(in){
    tile_size = 32;
    filter_threshold = 100;
}

PeakFinder::PeakFinder(ImageBuffer *in, int t, int threshold) : ImageProcessor(in){
    tile_size = t;
    filter_threshold = threshold;
}

void PeakFinder::process(){
    int h,w;
    int temp = 0;

    int pixel_color;
    int pixel_w = 0;
    int pixel_h = 0;

    int width = In->getWidth();
    int height = In->getHeight();

    int tile_padding = 3;
    vector<vector < int> > sat;

    peak_count = 0;

    ImageBuffer *mask = new ImageBuffer(In->getWidth(), In->getHeight());
    mask->set();

    //copy image to mask
    for(w=0; w < In->getWidth(); w++){
        for (h=0; h <In->getHeight(); h++){
            mask->setPixel(w,h,In->getPixel(w,h));
        }
    }

    //vector voor geheugen gebruik ipv array
    for(w = 0; w < width; w++){
        vector<int> temp;
        for(h = 0; h < height; h++){
           temp.push_back(255);
        }
        sat.push_back(temp);
    }

    for(w = 1; w < width; w++){
        for(h = 1; h < height; h++){
            sat[w][h] = mask->getPixel(w,h) + sat[w-1][h] + sat[w][h-1] - sat[w-1][h-1];
        }
    }

    //doorloop de de spectrogram per tile
    for(int tile_width = 0; tile_width*tile_size < width; tile_width++){
        for(int tile_height = 0; tile_height*tile_size < height; tile_height++){


            //Het doorlopen van alle tegels
            pixel_color = 255;
            temp = 255;

            for(int i=0; i < tile_size; i++){
                for(int j=0;j < tile_size;j++){
                    w = tile_width*tile_size+j;
                    h = tile_height*tile_size+i;

                    if(h > tile_padding && w > tile_padding && h < height-tile_padding && w < width-tile_padding){
                        temp = (sat[w-tile_padding][h-tile_padding] + sat[w+tile_padding][h+tile_padding] - sat[w-tile_padding][h+tile_padding] - sat[w+tile_padding][h-tile_padding])/(tile_padding*tile_padding*4);
                    }

                    //zoek de donkerste pixel binnen de tegel. deze heeft de grootste intesiteit
                    if(temp < pixel_color){
                        pixel_color = temp;
                        pixel_h = h;
                        pixel_w = w;
                    }
                    //In->setPixel(w,h, mask->getPixel(w,h));
                }
            }
            //marge om aantal punten lager te houden. Anders punt op elke tegel
            if(pixel_color < filter_threshold){
                mark(pixel_w, pixel_h);
            }

        }
    }

    cout << "Peaks found: " << peak_count << endl;
}

void PeakFinder::mark(int x, int y){
    int pos_x, pos_y, dot_size;

    //groot van het stipje`
    dot_size = 5;

    for(int i=0 ;i<dot_size; i++ ){
        for(int j=0; j<dot_size; j++){
            pos_x = x-((dot_size-1)/2)+i;
            pos_y = y-((dot_size-1)/2)+j;

            if(pos_x < In->getWidth() && pos_y < In->getHeight() && pos_x > 0 && pos_y > 0){
                In->setPixel(pos_x, pos_y, 0);
            }
        }
    }
    addPeak(pos_x,pos_y);
}

void PeakFinder::addPeak(int x, int y){
    peak_count++;
    peaks.push_back(make_pair(x,y));
}

vector< PairOfPeaks*> PeakFinder::getPairOfPeaks(){
    return pairsOfPeaks;
}

void PeakFinder::combinePeaks(){

    for(unsigned int i=0; i < peaks.size(); i++){
        int min_distance = 200;

        for(unsigned int j = 0;j < peaks.size(); j++){
            if(i != j){
                int distance = sqrt(pow(peaks[i].first-peaks[j].first, 2)+pow(peaks[i].second-peaks[j].second, 2));
                if(distance < min_distance){
                    pairsOfPeaks.push_back(new PairOfPeaks(peaks[i], peaks[j]));
                }
            }
        }
    }

//    for(set<string>::iterator it = hashes.begin(); it!=hashes.end(); it++){
//        for(int j = 0; j < samples.size(); j++){
//            if(*it == samples[j]){
//                match++;
//            }
//        }
//    }
//    cout << "Matches: " << match/pairs.size()*100 << "%" << endl;


}




/*

    int16_t sum,i,j,w,h, x, y,temp;

    ImageBuffer *mask = new ImageBuffer(In->getWidth(), In->getHeight());
    mask->set();

    //invert the image
    for(w=0; w < In->getWidth(); w++){
        for (h=0; h < In->getHeight(); h++){
            In->setPixel(w,h,255-In->getPixel(w,h));
        }
    }

    for(w=0; w< In->getWidth(); w++){
        for (h=0; h<In->getHeight(); h++){
            sum=0;
            for(i=0 ;i<7; i++ ){
                for(j=0; j<7; j++){
                    y=h-i+3; x=w-j+3;
                    if(y<0) y=0;
                    if(y>In->getHeight()-3) y=In->getHeight()-3;
                    if(x<0) x=0;
                    if(x>WIDTH-3) x=WIDTH-3;
                    //sum+=kernel[i][j]*(int16_t)(In->Content[y][x]);
                    sum = sum < In->getPixel(x,y) ? In->getPixel(x,y) : sum;
                }
            }
            mask->setPixel(w,h, sum);
        }
    }

    for(w=0;  w< In->getWidth(); w++){
        for (h=0; h<In->getHeight(); h++){
            temp = In->getPixel(w,h)-mask->getPixel(w,h);
            if(temp == 0 && In->getPixel(w,h) > 0)
                temp = In->getPixel(w,h);
            if(temp < 0){
                mask->setPixel(w,h,0);
            }else{
                int pos_x, pos_y, dot_size;
                int x = w;
                int y = h;
                //groot van het stipje
                dot_size = 5;

                for(int i=0 ;i<dot_size; i++ ){
                    for(int j=0; j<dot_size; j++){
                        pos_x = x-((dot_size-1)/2)+i;
                        pos_y = y-((dot_size-1)/2)+j;

                        if(pos_x < In->getWidth() && pos_y < In->getHeight() && pos_x > 0 && pos_y > 0){
                            mask->setPixel(pos_x, pos_y, 255);
                        }
                    }
                }
            }

        }
    }

    //set mask back to image
    for(w=0; w< In->getWidth(); w++){
        for (h=0; h<In->getHeight(); h++){
            In->setPixel(w,h,mask->getPixel(w,h));
        }
    }

  */
