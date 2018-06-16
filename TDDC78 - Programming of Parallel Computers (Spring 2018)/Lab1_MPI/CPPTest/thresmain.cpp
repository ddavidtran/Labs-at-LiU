#include <mpi.h>
#include <iostream>
#include "thresfilter.h"
#include <cmath>
#include <cstdlib>
#include <cstdio>
#include <cstring>
#include <algorithm>
#include <array>
#include <iterator>

#define MAX_X 1.33
#define Pi 3.14159

/* maximum number of pixels in a picture */
#define MAX_PIXELS (3000*3000)

int read_ppm (const char * fname, int * xpix, int * ypix, int * max, unsigned char * data);
int write_ppm (const char * fname, int xpix, int ypix, unsigned char * data);

int main(int argc, char *argv[]) {

    MPI_Init(nullptr, nullptr);
    int rank{}, world{}, root{0};

    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &world);

    std::cout << "Rank " << rank << " out of " << world << "\n";
    int radius;
    int xsize, ysize, colmax;
    unsigned char* src;
    unsigned char* newsrc;

    double t1{0.0}, t2{0.0};
    t1 = MPI_Wtime();

    struct timespec stime{}, etime{};

    if(rank == root) {

        src = new unsigned char[MAX_PIXELS * 3];
        newsrc = new unsigned char[MAX_PIXELS * 3];

        if (argc != 3) {
          fprintf(stderr, "Usage: %s infile outfile\n", argv[0]);
          exit(1);
          }


          if(read_ppm (argv[1], &xsize, &ysize, &colmax, src) != 0)
              exit(1);

          if (colmax > 255) {
          fprintf(stderr, "Too large maximum color-component value\n");
          exit(1);
        }

        printf("Has read the image, generating coefficients\n");

        //clock_gettime(CLOCK_REALTIME, &stime);
    }



    MPI_Bcast(&xsize, 1, MPI_INT, root, MPI_COMM_WORLD);
    MPI_Bcast(&ysize, 1, MPI_INT, root, MPI_COMM_WORLD);

    int ysplit = ysize/world;
    int rest = ysize%world;
    int give = rest==0 ? 0 : 1;
    int steal = world - 1 - rest;
    int* sendcounts = new int[world];
    int* displace = new int[world];
    for(int i=0; i<world; i++) {
        sendcounts[i] = (ysplit+give) * xsize * 3;
        displace[i] = (ysplit+give+1) * xsize * 3 * i;
    }

    if(rest != 0)
        sendcounts[world-1] = (ysplit - steal) * xsize * 3;

    auto dst = new unsigned char[sendcounts[rank]];

    MPI_Scatterv(src, sendcounts, displace, MPI_UNSIGNED_CHAR, dst, sendcounts[root], MPI_UNSIGNED_CHAR, root, MPI_COMM_WORLD);


    printf("Calling filter\n");

    unsigned int local_sum{0},sum{0};
    for(int i = 0; i < sendcounts[rank]; i++){
      local_sum += dst[i];
    }
    MPI_Allreduce(&local_sum, &sum, 1, MPI_INT, MPI_SUM, MPI_COMM_WORLD);

    unsigned char mean = static_cast<unsigned char>(sum/(xsize*ysize));
    //unsigned char mean = sum/(xsize*ysize);

    thresfilter(xsize, sendcounts[rank]/(xsize*3), dst, mean);
    std::cout << "Threshold: " << static_cast<int>(mean) << std::endl;

    MPI_Gather(dst, sendcounts[rank], MPI_UNSIGNED_CHAR, newsrc, sendcounts[root], MPI_UNSIGNED_CHAR, root, MPI_COMM_WORLD);
    if(rank == root) {

        printf("Writing output file\n");

        //clock_gettime(CLOCK_REALTIME, &etime);
        //printf("Filtering took: %g secs\n", (etime.tv_sec - stime.tv_sec) +
                                            //1e-9 * (etime.tv_nsec - stime.tv_nsec));
        if (write_ppm(argv[2], xsize, ysize, newsrc) != 0)
            return 1;
    }
    t2 = MPI_Wtime();
    double result = t2-t1;
    printf("MPI_Wtime for rank %d took %g\n", rank, result);
    fflush(stdout);


    if(root == rank){
      delete[] src;
      delete[] newsrc;
    }
    delete[] dst;

    MPI_Finalize();
    return (0);
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
