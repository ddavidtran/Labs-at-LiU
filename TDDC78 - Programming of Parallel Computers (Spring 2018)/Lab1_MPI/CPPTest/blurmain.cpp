#include <mpi.h>
#include <iostream>
#include "blurfilter.h"
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
void get_gauss_weights(int n, double* weights_out);

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

#define MAX_RAD 1000
    struct timespec stime{}, etime{};
    double w[MAX_RAD];

    t1 = MPI_Wtime();

    if(rank == root) {

        src = new unsigned char[MAX_PIXELS * 3];
        newsrc = new unsigned char[MAX_PIXELS * 3];

        /* Take care of the arguments */
        if (argc != 4) {
            fprintf(stderr, "Usage: %s radius infile outfile\n", argv[0]);
            exit(1);
        }

        radius = atoi(argv[1]);
        if ((radius > MAX_RAD) || (radius < 1)) {
            fprintf(stderr, "Radius (%d) must be greater than zero and less then %d\n", radius, MAX_RAD);
            exit(1);
        }

        /* read file */
        if (read_ppm(argv[2], &xsize, &ysize, &colmax, src) != 0)
            exit(1);

        if (colmax > 255) {
            fprintf(stderr, "Too large maximum color-component value\n");
            exit(1);
        }

        printf("Has read the image, generating coefficients\n");

        //clock_gettime(CLOCK_REALTIME, &stime);
    }

    /* filter */

    MPI_Bcast(&xsize, 1, MPI_INT, root, MPI_COMM_WORLD);
    MPI_Bcast(&ysize, 1, MPI_INT, root, MPI_COMM_WORLD);
    MPI_Bcast(&radius, 1, MPI_INT, root, MPI_COMM_WORLD);
    MPI_Bcast(&w, 1, MPI_INT, root, MPI_COMM_WORLD);

    get_gauss_weights(radius, w);

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

    auto overlap_top_recv = new unsigned char[radius*xsize*3];
    auto overlap_bottom_recv = new unsigned char[radius*xsize*3];

    int bot_from = ysplit*xsize*3 - radius*xsize*3;
    int bot_to = ysplit*xsize*3;

    int top_to = radius*xsize*3;
    auto overlap_bottom_send = new unsigned char[radius*xsize*3];
    auto overlap_top_send = new unsigned char[radius*xsize*3];
	

   if(world != 1){	
    if(rank == root){      
	for(int i=0; i<xsize*radius*3; i++){
        overlap_bottom_send[i] = dst[i + (sendcounts[rank] - xsize*radius*3)];
      	}
	
        MPI_Send(overlap_bottom_send, radius*xsize*3, MPI_UNSIGNED_CHAR, rank+1, 0, MPI_COMM_WORLD);
        MPI_Recv(overlap_bottom_recv, radius*xsize*3, MPI_UNSIGNED_CHAR, rank+1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    }
    else if(rank == world-1){
        for(int i=0; i<xsize*radius*3; i++){
          overlap_top_send[i] = dst[i];
        }

        MPI_Recv(overlap_top_recv, radius*xsize*3, MPI_UNSIGNED_CHAR, rank-1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        MPI_Send(overlap_top_send, radius*xsize*3, MPI_UNSIGNED_CHAR, rank-1, 0, MPI_COMM_WORLD);
    }
    else{
        for(int i=0; i<xsize*radius*3; i++){
          overlap_top_send[i] = dst[i];
          overlap_bottom_send[i] = dst[i + (sendcounts[rank] - xsize*radius*3)];
        }

        MPI_Recv(overlap_top_recv, radius*xsize*3, MPI_UNSIGNED_CHAR, rank-1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        MPI_Send(overlap_bottom_send, radius*xsize*3, MPI_UNSIGNED_CHAR, rank+1, 0, MPI_COMM_WORLD);
        MPI_Recv(overlap_bottom_recv, radius*xsize*3, MPI_UNSIGNED_CHAR, rank+1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        MPI_Send(overlap_top_send, radius*xsize*3, MPI_UNSIGNED_CHAR, rank-1, 0, MPI_COMM_WORLD);
     }
   }

    printf("Calling filter\n");

    blurfilter(xsize, sendcounts[rank]/(xsize*3), overlap_top_recv, overlap_bottom_recv, dst, radius, w);

    MPI_Gather(dst, sendcounts[rank], MPI_UNSIGNED_CHAR, newsrc, sendcounts[root], MPI_UNSIGNED_CHAR, root, MPI_COMM_WORLD);

    if(rank == root) {
        /* write result */
        printf("Writing output file\n");
        //clock_gettime(CLOCK_REALTIME, &etime);
        //printf("Filtering took: %g secs\n", (etime.tv_sec - stime.tv_sec) +
                                            //1e-9 * (etime.tv_nsec - stime.tv_nsec));
        if (write_ppm(argv[3], xsize, ysize, newsrc) != 0)
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
    delete[] overlap_bottom_send;
    delete[] overlap_top_send;
    delete[] overlap_bottom_recv;
    delete[] overlap_top_recv;
    delete[] dst;

    MPI_Finalize();
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
