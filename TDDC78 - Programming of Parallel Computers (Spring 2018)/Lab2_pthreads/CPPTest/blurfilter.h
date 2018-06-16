/*
  File: blurfilter.h

  Declaration of pixel structure and blurfilter function.
    
 */

#ifndef _BLURFILTER_H_
#define _BLURFILTER_H_

struct blur_data{
public:
    unsigned char* src;
    unsigned char* newsrc;
    int from;
    int to;
    int xsize;
    int ysize;
    int radius;
    double* w;
};

void* blurfilter(void* blurargs);

#endif
