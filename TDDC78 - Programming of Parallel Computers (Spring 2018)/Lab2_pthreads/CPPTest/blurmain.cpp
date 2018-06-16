#include <iostream>
#include "blurfilter.h"
#include <cmath>
#include <cstdlib>
#include <cstdio>
#include <cstring>
#include <algorithm>
#include <array>
#include <iterator>
#include <thread>
#include <vector>
#include <sstream>

#define MAX_X 1.33
#define Pi 3.14159

/* maximum number of pixels in a picture */
#define MAX_PIXELS (3000*3000)

int read_ppm (const char * fname, int * xpix, int * ypix, int * max, unsigned char * data);
int write_ppm (const char * fname, int xpix, int ypix, unsigned char * data);
void get_gauss_weights(int n, double* weights_out);

int main(int argc, char *argv[]) {
    std::istringstream ss(argv[4]);
    int x;
    if (!(ss >> x))
        std::cerr << "Invalid number " << argv[4] << '\n';

    unsigned int num_processor = static_cast<unsigned int>(x);
    std::cout << "Number of processors: " <<  num_processor << std::endl;
    std::vector<pthread_t> threads(num_processor);

    int radius;
    int xsize, ysize, colmax;
    unsigned char* src;
    unsigned char* newsrc;

#define MAX_RAD 1000
    struct timespec stime{}, etime{};
    double w[MAX_RAD];


    src = new unsigned char[MAX_PIXELS * 3];
    newsrc = new unsigned char[MAX_PIXELS * 3];

    /* Take care of the arguments */
    if (argc != 5) {
        std::cerr << "Usage " << argv[0] << " radius infile outfile" << std::endl;
        exit(1);
    }

    radius = atoi(argv[1]);
    if ((radius > MAX_RAD) || (radius < 1)) {
        std::cerr << "Radius " << radius << " must be greater than zero and less then " << MAX_RAD << std::endl;
        exit(1);
    }

    /* read file */
    if (read_ppm(argv[2], &xsize, &ysize, &colmax, src) != 0)
        exit(1);

    if (colmax > 255) {
        std::cerr << "Too large maximum color-component value" << std::endl;
        exit(1);
    }

    std::cout << "Has read the image, generating coefficients\n";

    clock_gettime(CLOCK_REALTIME, &stime);

    /* filter */

    get_gauss_weights(radius, w);

    std::cout << "Calling filter\n";

    int ysplit = ysize/num_processor;
    int rest = ysize%num_processor;
    std::vector<unsigned int> splitcounts(num_processor, ysplit);
    splitcounts[num_processor-1] += rest;
    std::copy(splitcounts.begin(), splitcounts.end(), std::ostream_iterator<unsigned int>(std::cout, " "));
    std::cout << std::endl << rest << "\n";

    int from{0}, to{0};
    struct blur_data data[num_processor];

    for(int i = 0; i < num_processor; i++){
        from = to;
        to = to + splitcounts[i];
        data[i].src = src;
        data[i].from = from;
        data[i].to = to;
        data[i].xsize = xsize;
        data[i].ysize = ysize;
        data[i].radius = radius;
        data[i].newsrc = newsrc;
        data[i].w = w;
        std::cout << "main() : Creating thread " << i << std::endl;
        pthread_create(&threads[i], nullptr, blurfilter, (void*)&data[i]);
    }

    void *status;
    for(auto t : threads){
        pthread_join(t, &status);
    }


    /* write result */
    std::cout << "Writing output file\n";

    clock_gettime(CLOCK_REALTIME, &etime);

    std::cout << "Filtering took " << (etime.tv_sec - stime.tv_sec) +
                                      1e-9 * (etime.tv_nsec - stime.tv_nsec) << " secs" << std::endl;


    if (write_ppm(argv[3], xsize, ysize, newsrc) != 0)
        return 1;

    delete[] src;
    delete[] newsrc;

    return (0);
}


/* Generate an array of weights for the gaussian filter. */
/* Input: n - number of weights to generate              */
/* Output: weights_out - array of weights. The element [0] */
/*  should be used for the central pixel, elements [1..n] */
/*  should be used for the pixels on a distance [1..n]  */
void get_gauss_weights(int n, double* weights_out) {
    double x;
    int i;

    for( i = 0; i < n+1; i++) {
        x = (double)i * MAX_X/n;
        weights_out[i] = exp(-x*x * Pi);
    }
}


/* Function: read_ppm - reads data from an image file in PPM format.
   Input: fname - name of an image file in PPM format to read.
   Output:
      xpix, ypix - size of the image in x & y directions
      max - maximum intensity in the picture
      data - color data array. MUST BE PREALLOCATED to at least MAX_PIXELS*3 bytes.
   Returns: 0 on success.
 */
int read_ppm (const char * fname, int * xpix, int * ypix, int * max, unsigned char * data) {
    char ftype[40];
    char ctype[40] = "P6";
    char line[80];
    int err;

    FILE * fp;
    err = 0;

    if (fname == nullptr) fname = "\0";
    fp = fopen (fname, "r");
    if (fp == nullptr) {
        fprintf (stderr, "read_ppm failed to open %s: %s\n", fname,
                 strerror (err));
        return 1;
    }

    fgets(line, 80, fp);
    sscanf(line, "%s", ftype);

    while (fgets(line, 80, fp) && (line[0] == '#'));

    sscanf(line, "%d%d", xpix, ypix);
    fscanf(fp, "%d\n", max);

    if(*xpix * *ypix > MAX_PIXELS) {
        fprintf (stderr, "Image size is too big\n");
        return 4;
    };


    if (strncmp(ftype, ctype, 2) == 0) {
        if (fread (data, sizeof (char), *xpix * *ypix * 3, fp) !=
            *xpix * *ypix * 3) {
            perror ("Read failed");
            return 2;
        }
    } else {
        fprintf (stderr, "Wrong file format: %s\n", ftype);
    }

    if (fclose (fp) == EOF) {
        perror ("Close failed");
        return 3;
    }


    return 0;

}

/* Function: write_ppm - write out an image file in PPM format.
   Input:
      fname - name of an image file in PPM format to write.
      xpix, ypix - size of the image in x & y directions
      data - color data.
   Returns: 0 on success.
 */
int write_ppm (const char * fname, int xpix, int ypix, unsigned char * data) {

    FILE * fp;
    int err = 0;

    if (fname == nullptr) fname = "\0";
    fp = fopen (fname, "w");
    if (fp == nullptr) {
        fprintf (stderr, "write_ppm failed to open %s: %s\n", fname,
                 strerror (err));
        return 1;
    }

    fprintf (fp, "P6\n");
    fprintf (fp, "%d %d 255\n", xpix, ypix);
    if (fwrite (data, sizeof (char), xpix*ypix*3, fp) != xpix*ypix*3) {
        perror ("Write failed");
        return 2;
    }
    if (fclose (fp) == EOF) {
        perror ("Close failed");
        return 3;
    }
    return 0;
}
