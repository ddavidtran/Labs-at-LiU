#include "thresfilter.h"
#include <pthread.h>

void* thresfilter(void* thres){
  struct thres_data *data;
  data = (struct thres_data*) thres;
  unsigned char psum;
  for(int i = data->from; i < data->to; i+=3){
    psum = data->src[i] + data->src[i+1] + data->src[i+2];
    if(data->avg > psum)
      data->src[i] = data->src[i+1] = data->src[i+2] = 0;
    else
      data->src[i] = data->src[i+1] = data->src[i+2] = 255;
  }
  pthread_exit(nullptr);
}
