#include <iostream>
#include "thresfilter.h"
#include <cmath>
#include <cstdlib>
#include <cstdio>
#include <cstring>
#include <algorithm>
#include <array>
#include <iterator>
#include <pthread.h>
#include <thread>
#include <vector>
#include <sstream>

#define MAX_X 1.33
#define Pi 3.14159

/* maximum number of pixels in a picture */
#define MAX_PIXELS (3000*3000)

int read_ppm (const char * fname, int * xpix, int * ypix, int * max, unsigned char * data);
int write_ppm (const char * fname, int xpix, int ypix, unsigned char * data);
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
void* average_pix(void* avg);

struct avg_data{
public:
    unsigned char* src;
    int from;
    int to;
    unsigned int *avg;
};

int main(int argc, char *argv[]) {
    std::istringstream ss(argv[3]);
    int x;
    if (!(ss >> x))
        std::cerr << "Invalid number " << argv[3] << '\n';

    unsigned int num_processor = static_cast<unsigned int>(x);
    std::cout << "Number of processors: " <<  num_processor << std::endl;
    std::vector<pthread_t> threads(num_processor);
    
    int xsize, ysize, colmax;
    unsigned char* src;
    unsigned char* newsrc;

    struct timespec stime{}, etime{};

    src = new unsigned char[MAX_PIXELS * 3];

    if (argc != 4) {
      std::cerr << "Usage: %s infile outfile\n" << argv[0] << std::endl;
      exit(1);
      }


      if(read_ppm (argv[1], &xsize, &ysize, &colmax, src) != 0)
          exit(1);

      if (colmax > 255) {
          std::cerr << "Too large maximum color-component value\n" << std::endl;
    }

    std::cout << "Has read the image, generating coefficients " << std::endl;

    clock_gettime(CLOCK_REALTIME, &stime);

    int ysplit = ysize/num_processor;
    int rest = ysize%num_processor;
    std::vector<unsigned int> splitcounts(num_processor, ysplit*xsize*3);
    splitcounts[num_processor-1] += rest*xsize*3;
    std::copy(splitcounts.begin(), splitcounts.end(), std::ostream_iterator<unsigned int>(std::cout, " "));

    std::cout << std::endl;
    std::cout << "Calling filter" << std::endl;

    unsigned int avg{0};

    int from{0}, to{0};
    struct avg_data data[num_processor];

    for(int i = 0; i < num_processor; i++){
        from = to;
        to = to + splitcounts[i];
        data[i].src = src;
        data[i].from = from;
        data[i].to = to;
        data[i].avg = &avg;
        std::cout << "main() : Creating thread " << i << std::endl;
        pthread_create(&threads[i], nullptr, average_pix, (void*)&data[i]);
    }

    void *status;
    for(auto t : threads){
        pthread_join(t, &status);
    }

    unsigned int thres_val = avg/(xsize*ysize*3);

    from = 0;
    to = 0;

    struct thres_data thres[num_processor];
    for(int i = 0; i < num_processor; i++){
        from = to;
        to = to + splitcounts[i];
        thres[i].src = src;
        thres[i].from = from;
        thres[i].to = to;
        thres[i].avg = thres_val;
        pthread_create(&threads[i], nullptr, thresfilter, (void*)&thres[i]);
    }

    for(auto t : threads){
        pthread_join(t, &status);
    }

    std::cout << "Calling filter" << std::endl;

    clock_gettime(CLOCK_REALTIME, &etime);

    std::cout << "Filtering took " << (etime.tv_sec - stime.tv_sec) +
                                     1e-9 * (etime.tv_nsec - stime.tv_nsec) << " secs" << std::endl;

    if (write_ppm(argv[2], xsize, ysize, src) != 0)
        return 1;

    delete[] src;

    return (0);
}


void* average_pix(void* avg){
    struct avg_data *data;
    data = (struct avg_data*) avg;
    unsigned int localsum{0};
    for(int i = data->from; i < data->to; i++)
        localsum += data->src[i];

    pthread_mutex_lock( &mutex );
    *(data->avg) += localsum;
    pthread_mutex_unlock( &mutex );

    pthread_exit(nullptr);
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
