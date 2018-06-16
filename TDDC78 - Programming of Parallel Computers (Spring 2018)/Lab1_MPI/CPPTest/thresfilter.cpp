#include "thresfilter.h"

void thresfilter(const int xsize, const int ysize, unsigned char* src, unsigned char mean){

unsigned char psum;
for(int i = 0; i < xsize*ysize*3; i+=3){
  psum = src[i] + src[i+1] + src[i+2];
  
  if(static_cast<int>(mean) > static_cast<int>(psum))
     src[i] = src[i+1] = src[i+2] = 0;
  else
    src[i] = src[i+1] = src[i+2] = 255;
  }

}
