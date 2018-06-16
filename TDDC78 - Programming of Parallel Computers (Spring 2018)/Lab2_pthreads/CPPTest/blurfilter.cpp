/*
  File: blurfilter.c

  Implementation of blurfilter function.

 */
#include <cstdio>
#include "blurfilter.h"
#include <ctgmath>
#include <iostream>
#include <cfloat>


void* blurfilter(void* blurargs){
    struct blur_data *data;
    data = (struct blur_data*) blurargs;

    int x{}, y{};
    double wc{}, n{};

    for(int row = data->from; row < data->to; row++) {
        for(int c = 0; c < data->xsize; c++) {
            double r = 0;
            double g = 0;
            double b = 0;

            n = 0;
            for (x = -data->radius; x <= data->radius; x++) {
                for (y = -data->radius; y <= data->radius; y++) {

                    wc = data->w[std::max(abs(x), abs(y))];

                    if(c+x >= 0 && c+x < data->xsize && (y+row) >= 0 && (y+row) < data->ysize){
                        r += wc * static_cast<double>(data->src[data->xsize*3 * (row+y) + (c+x)*3 + 0]);
                        g += wc * static_cast<double>(data->src[data->xsize*3 * (row+y) + (c+x)*3 + 1]);
                        b += wc * static_cast<double>(data->src[data->xsize*3 * (row+y) + (c+x)*3 + 2]);
                        n += wc;
                    }
                }
            }

            data->newsrc[data->xsize*row*3 + c*3 + 0] = (unsigned char)(r / n);
            data->newsrc[data->xsize*row*3 + c*3 + 1] = (unsigned char)(g / n);
            data->newsrc[data->xsize*row*3 + c*3 + 2] = (unsigned char)(b / n);

        }
    }
    pthread_exit(nullptr);
}
