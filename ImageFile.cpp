/* ImageFile.cpp v1.0: Class for reading from or writing to file.
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
#include "ImageFile.h"
#include "Parameters.h" 
#include <stdlib.h>
#include <stdio.h>
#include <iostream>

using namespace std;
  

ImageFile::ImageFile(char *name, ImageBuffer *buffer){
  sprintf(FileName, "%s",name);		
  Buffer=buffer;
  sprintf(Magicstr, "P5");
  sprintf(Widthstr, "%d",Buffer->getWidth());
  sprintf(Heightstr, "%d",Buffer->getHeight());
  sprintf(Resstr, "%d", 255);
}
    

void ImageFile::read(){
  u_char dummy;
  FILE *handle;
  
  handle = fopen(FileName, "r");
  fscanf(handle, "%s", Magicstr);
  fscanf(handle, "%s", Widthstr);
  fscanf(handle, "%s", Heightstr);
  fscanf(handle, "%s", Resstr);
  fread(&dummy, 1, 1, handle);
  fread((u_char *)Buffer->getContent(), WIDTH*HEIGHT, 1, handle);
  fclose(handle);
}


void ImageFile::write(){
	FILE* handle;
	
	handle = fopen(FileName, "w");
    fprintf (handle, "%s\n%s %s\n%s\n",Magicstr, Widthstr, Heightstr, Resstr);

    fwrite((u_char *)Buffer->getContent(), (Buffer->getWidth()*Buffer->getHeight()), 1, handle);
	fclose(handle);
}

