/* ImageProcessor.cpp v1.0: Base Class for Processing an image.
 * Copyright 2008 Pieter Burghouwt Haagse Hogeschool Rijswijk
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
#include <stdint.h>
#include "Parameters.h" 
#include "ImageBuffer.h"
#include "ImageProcessor.h"

TImageProcessor::TImageProcessor(TImageBuffer *in){
    In=in;
};

TImageProcessor::TImageProcessor(TImageBuffer *in, TImageBuffer *out){
    In=in;
    Out=out;
};


void TImageProcessor::process(void){
    //simple copy function, normally overrided in child classes
    uint16_t h, w;

    for(h=0; h<HEIGHT; h++){
        for(w=0; w<WIDTH; w++){
            Out->Content[h][w]=In->Content[h][w];
        }
    }
};
