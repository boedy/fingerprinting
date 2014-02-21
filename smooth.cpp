#include "smooth.h"

Smooth::Smooth(ImageBuffer *in, int s) : ImageProcessor(in)
{
    size = s;
}

void Smooth::process(){
    int16_t kernel[size][size];
    int16_t sum,i,j,w,h, x, y,kernal_r;

//    kernel[0][0]=1; kernel[0][1]= 1; kernel[0][2]=1;
//    kernel[1][0]=1; kernel[1][1]= 1; kernel[1][2]=1;
//    kernel[2][0]=1; kernel[2][1]= 1; kernel[2][2]=1;

    for(i = 0; i < size; i++){
        for(j = 0; j < size; j++){
            kernel[i][j] = 1;
        }
    }



    ImageBuffer *mask = new ImageBuffer(In->getWidth(), In->getHeight());
    mask->set();

    kernal_r = (size-1)/2;

    for(w=0; w< In->getWidth(); w++){
        for (h=0; h<In->getHeight(); h++){
            sum=0;
            for(i=0 ;i<size; i++ ){
                for(j=0; j<size; j++){
                    y=h-i+kernal_r; x=w-j+kernal_r;
                    if(y<0) y=0;
                    if(y>In->getHeight()-kernal_r) y=In->getHeight()-kernal_r;
                    if(x<0) x=0;
                    if(x > In->getWidth()-kernal_r) x=In->getWidth()-kernal_r;
                    sum+=kernel[i][j]*(int16_t)(In->getPixel(x,y));
                }
            }
            sum/=size*size;
            if(sum<100) sum=0;
            if(sum>255) sum=255;
            mask->setPixel(w,h,sum);
        }
    }

    //set mask back to image
    for(w=0; w < In->getWidth(); w++){
        for (h=0; h <In->getHeight(); h++){
            In->setPixel(w,h,mask->getPixel(w,h));
        }
    }
}
