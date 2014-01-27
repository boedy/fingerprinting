#ifndef IMAGEFILTER_H_
#define IMAGEFILTER_H_

#include "Parameters.h" 
#include "ImageBuffer.h"

class TImageFilter{
	protected:
	  TImageBuffer *In;
	  TImageBuffer *Out;
	public:
	  TImageFilter(TImageBuffer *in, TImageBuffer *out);
	  void process(void);
};

#endif /*IMAGEFILTER_H_*/
