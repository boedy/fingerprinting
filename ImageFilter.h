#ifndef IMAGEFILTER_H_
#define IMAGEFILTER_H_

#include "Parameters.h" 
#include "ImageBuffer.h"

class ImageFilter{
	protected:
      ImageBuffer *In;
      ImageBuffer *Out;
	public:
      ImageFilter(ImageBuffer *in, ImageBuffer *out);
	  void process(void);
};

#endif /*IMAGEFILTER_H_*/
