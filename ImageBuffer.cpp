/* ImageBuffer.cpp v1.0: Class for storing a grayscale picture.
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
#include "ImageBuffer.h" 
#include "Parameters.h" 


TImageBuffer::TImageBuffer(void){
    width = 640;
    height = 480;
}

TImageBuffer::TImageBuffer(int w, int h){
    width = w;
    height = h;

    canvas = new uint8_t[width*height];
}

uint8_t * TImageBuffer::getContent(){
    return &canvas[0];
}


void TImageBuffer::clear(){
    uint16_t w, h;

    for (h=0; h<HEIGHT; h++){
        for (w=0; w<WIDTH; w++){
            Content[h][w]=0;
        }
    }
}

void TImageBuffer::set(){
    int i;

    for(i=0; i < width*height;i++){
        canvas[i] = 0;
    }
}


uint8_t TImageBuffer::getPixel(int x, int y){
    return canvas[x+y*width];
}


void TImageBuffer::setPixel(int x, int y, uint8_t v){
    canvas[x+y*width] = v;
}

int TImageBuffer::getHeight(){
    return height;
}

int TImageBuffer::getWidth(){
    return width;
}
