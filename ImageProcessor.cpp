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

ImageProcessor::ImageProcessor(ImageBuffer *in){
    In=in;
}

ImageProcessor::ImageProcessor(ImageBuffer *in, ImageBuffer *out){
    In=in;
    Out=out;
}


void ImageProcessor::process(void){

}
