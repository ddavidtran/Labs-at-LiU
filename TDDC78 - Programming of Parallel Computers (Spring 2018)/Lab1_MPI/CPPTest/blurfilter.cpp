/*
  File: blurfilter.c

  Implementation of blurfilter function.

 */
#include <cstdio>
#include "blurfilter.h"
#include <mpi.h>
#include <ctgmath>
#include <iostream>
#include <cfloat>

pixel* pix(pixel* image, const int xx, const int yy, const int xsize)
{
  int off = xsize*yy + xx;

#ifdef DBG
  if(off >= MAX_PIXELS) {
    fprintf(stderr, "\n Terribly wrong: %d %d %d\n",xx,yy,xsize);
  }
#endif
  return (image + off);
}

void blurfilter(const int xsize, const int ysize, unsigned char* overlap_top, unsigned char* overlap_bot, unsigned char *dst, const int radius, const double w[]) {

    int rank{}, world{}, root{0};
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &world);

    int x{}, y{};
    double wc{}, n{};
    unsigned char* copy = new unsigned char[xsize*ysize*3];
    for(int i=0; i<xsize*ysize*3; i++){
      copy[i] = dst[i];
    }

    for(int row = 0; row < ysize; row++) {
        for(int c = 0; c < xsize; c++) {
            double r = 0;
            double g = 0;
            double b = 0;

            n = 0;
            for (x = -radius; x <= radius; x++) {
                for (y = -radius; y <= radius; y++) {

                    wc = w[std::max(abs(x), abs(y))];
                    if(c+x >= 0 && c+x < xsize){

                        if( row+y < 0  && rank != root){

                            r += wc * static_cast<double>(overlap_top[xsize * 3*(radius+y) + (c+x)*3 + 0]);
                            g += wc * static_cast<double>(overlap_top[xsize * 3*(radius+y) + (c+x)*3 + 1]);
                            b += wc * static_cast<double>(overlap_top[xsize * 3*(radius+y) + (c+x)*3 + 2]);
                            n += wc;

                        }
                        else if( row+y >= ysize && rank != world-1){

                            r += wc * static_cast<double>(overlap_bot[xsize * 3*y + (c+x)*3 + 0]);
                            g += wc * static_cast<double>(overlap_bot[xsize * 3*y + (c+x)*3 + 1]);
                            b += wc * static_cast<double>(overlap_bot[xsize * 3*y + (c+x)*3 + 2]);
                            n += wc;

                        }
                        else if( row+y < ysize && row+y >= 0){

                            r += wc * static_cast<double>(copy[xsize*3 * (row+y) + (c+x)*3 + 0]);
                            g += wc * static_cast<double>(copy[xsize*3 * (row+y) + (c+x)*3 + 1]);
                            b += wc * static_cast<double>(copy[xsize*3 * (row+y) + (c+x)*3 + 2]);
                            n += wc;

                        }
                    }

                }
            }
            /*if(rank == 0){
              std::cout << (r / n) << std::endl;
            }*/
            dst[xsize*row*3 + c*3 + 0] = (unsigned char)(r / n);
            dst[xsize*row*3 + c*3 + 1] = (unsigned char)(g / n);
            dst[xsize*row*3 + c*3 + 2] = (unsigned char)(b / n);

        }
    }
    delete[] copy;


}
