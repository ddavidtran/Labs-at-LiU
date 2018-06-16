/*
  File: blurfilter.h

  Declaration of pixel structure and blurfilter function.
    
 */

#ifndef _BLURFILTER_H_
#define _BLURFILTER_H_

/* NOTE: This structure must not be padded! */
typedef struct _pixel {
    unsigned char r,g,b;
} pixel;

void blurfilter(int xsize, int ysize, unsigned char* overlap_top, unsigned char* overlap_bot, unsigned char* dst, int radius, const double w[]);

#endif
