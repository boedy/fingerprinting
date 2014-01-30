#include "tdots.h"
#include <iostream>
#include <cmath>

using namespace std;

TDots::TDots(TImageBuffer *in, TImageBuffer *out) : TImageProcessor(in, out){
    tile_size = 64;
    filter_threshold = 100;
}

TDots::TDots(TImageBuffer *in, TImageBuffer *out, int t, int threshold) : TImageProcessor(in, out){
    tile_size = t;
    filter_threshold = threshold;
}

void TDots::process(){
    int h,w;
    int temp = 0;

    int pixel_color;
    int pixel_w = 0;
    int pixel_h = 0;

    int width = In->getWidth();
    int height = In->getHeight();

    int tile_padding = 3;
    vector<vector < int> > sat;

    dot_count = 0;

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
            sat[w][h] = In->getPixel(w,h) + sat[w-1][h] + sat[w][h-1] - sat[w-1][h-1];
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
                    Out->setPixel(w,h, In->getPixel(w,h));
                }
            }
            //marge om aantal punten lager te houden. Anders punt op elke tegel
            if(pixel_color < filter_threshold){
                mark(pixel_w, pixel_h);
            }

        }
    }
    cout << "Dots placed: " << dot_count << endl;
}

void TDots::mark(int x, int y){
    int pos_x, pos_y, dot_size;

    //groot van het stipje
    dot_size = 5;

    for(int i=0 ;i<dot_size; i++ ){
        for(int j=0; j<dot_size; j++){
            pos_x = x-((dot_size-1)/2)+i;
            pos_y = y-((dot_size-1)/2)+j;

            if(pos_x < In->getWidth() && pos_y < In->getHeight() && pos_x > 0 && pos_y > 0){
                Out->setPixel(pos_x, pos_y, 0);
            }
        }
    }
    setPoint(pos_x,pos_y);
}

void TDots::setPoint(int x, int y){
    dot_count++;
    points.push_back(make_pair(x,y));
}

void TDots::pairCalculation(){
    for(unsigned int i=0; i < points.size(); i++){
        int min_distance = 99999999;

        pair< pair<int,int>, pair<int,int> > closest_pair;
        for(unsigned int j = 0;j < points.size(); j++){
            if(i != j){
                int temp = sqrt(pow(points[i].first-points[j].second, 2)+pow(points[i].first-points[j].second, 2));
                if(temp < min_distance){
                    min_distance = temp;
                    closest_pair.first = points[i];
                    closest_pair.second = points[j];
                }
            }
        }
        //todo:verwijderen dubbel paar
        pairs.push_back(closest_pair);
    }
    for(unsigned int i = 0; i < pairs.size(); i++){
        int freqEstimate = pairs[i].first.second/10;
        int deltaFreq = abs(pairs[i].first.second-pairs[i].second.second);
        int deltaTime = abs(pairs[i].first.first-pairs[i].second.first);
        cout << freqEstimate << "::" << deltaFreq << "::" << deltaTime << endl;
    }
//    cout << "("<< pairs[i].first.first << "," << pairs[i].first.second << ")" << "("<< pairs[i].second.first << "," << pairs[i].second.second << ")" << endl;
}
