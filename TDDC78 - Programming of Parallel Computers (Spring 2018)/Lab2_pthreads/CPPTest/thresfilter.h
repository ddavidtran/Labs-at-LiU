/*
  File: thresfilter.h

  Declaration of pixel structure and thresfilter function.

 */
#ifndef _THRESFILTER_H_
#define _THRESFILTER_H_
struct thres_data{
public:
    unsigned char* src;
    int from;
    int to;
    unsigned int avg;
};
void* thresfilter(void* thres);

#endif
