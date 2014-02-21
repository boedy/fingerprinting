#include "peakfinder.h"

PeakFinder::PeakFinder(ImageBuffer* in) : ImageProcessor(in){

}

void PeakFinder::process(){
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
}
