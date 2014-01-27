/* ImageFile.h v1.0: Class for reading from or writing to file.
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
#ifndef IMAGEFILE_H_
#define IMAGEFILE_H_

#include "ImageBuffer.h" 
#include "Parameters.h"


class TImageFile{

  private:
    char Magicstr[10];
    char Widthstr[10];
    char Heightstr[10];
    char Resstr[10];
    char FileName[80];
    TImageBuffer *Buffer;

  public:  
    TImageFile(char *name, TImageBuffer *buffer);
    void getMagicstr(char *str);
    void getWidthstr(char *str);
    void getHeightstr(char *str);
    void getResstr(char *str);
    void setMagicstr(char *str);
    void setWidthstr(char *str);
    void setHeightstr(char *str);
    void setResstr(char *str);
    void read();
    void write();	
};


#endif /*IMAGEFILE_H_*/
