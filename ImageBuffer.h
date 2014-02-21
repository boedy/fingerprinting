/* ImageBuffer.h v1.0: Class for storing a grayscale picture.
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
#ifndef IMAGEBUFFER_
#define IMAGEBUFFER_

#include "Parameters.h" 
#include <stdint.h>

class ImageBuffer{
public:
    ImageBuffer(void);
    ImageBuffer(int height, int width);
    uint8_t * getContent(void);
    void clear(void);
    void set(void);
    uint8_t getPixel(int x, int y);
    void setPixel(int x, int y, uint8_t v);
    int getHeight();
    int getWidth();
private:
    uint8_t Content[HEIGHT][WIDTH];
    int width, height;
    uint8_t* canvas;
};


#endif /*IMAGEBUFFER_*/
