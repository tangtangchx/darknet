#ifndef IM2COL_H
#define IM2COL_H

//add for im2col.c 
//add 2018.1.4 from wenfang.min 
#define MAX_CONV 368640 


void im2col_cpu(float* data_im,
        int channels, int height, int width,
        int ksize, int stride, int pad, float* data_col);

void im2col_cpu_cw(float* data_im, float* data_im_pad,
	int channels, int height, int width,
	int ksize, int stride, int pad, float* data_col);
#ifdef GPU

void im2col_gpu(float *im,
         int channels, int height, int width,
         int ksize, int stride, int pad,float *data_col);

#endif
#endif
