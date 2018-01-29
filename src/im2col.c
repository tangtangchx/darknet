#include "im2col.h"
#include <stdio.h>
float im2col_get_pixel(float *im, int height, int width, int channels,
                        int row, int col, int channel, int pad)
{
    row -= pad;
    col -= pad;

    if (row < 0 || col < 0 ||
        row >= height || col >= width) return 0;
    return im[col + width*(row + height*channel)];
}

//From Berkeley Vision's Caffe!
//https://github.com/BVLC/caffe/blob/master/LICENSE
//������feature mapת��Ϊ����
//data_im:��������
//channels:��������ͨ����
	//height���������ݵĸ�
	//width���������ݵĿ�
	//ksize������˵Ĵ�С
	//stride��������ƶ��Ĳ��������Ķ���stride = 1������
	//pad��paddingʱ����Ҫ���ĳ��ȣ�padding�ĳ���Ϊksize / 2
void im2col_cpu(float* data_im,
     int channels,  int height,  int width,
     int ksize,  int stride, int pad, float* data_col) 
{
    int c,h,w;
    int height_col = (height + 2*pad - ksize) / stride + 1;   //�����������h
    int width_col = (width + 2*pad - ksize) / stride + 1; //�����������w

    int channels_col = channels * ksize * ksize;//im2col��ľ���������3*3*3=27
    for (c = 0; c < channels_col; ++c) {
        int w_offset = c % ksize;
        int h_offset = (c / ksize) % ksize;
        int c_im = c / ksize / ksize;
        for (h = 0; h < height_col; ++h) {
            for (w = 0; w < width_col; ++w) {
                int im_row = h_offset + h * stride;
                int im_col = w_offset + w * stride;
                int col_index = (c * height_col + h) * width_col + w;

                data_col[col_index] = im2col_get_pixel(data_im, height, width, channels,
                        im_row, im_col, c_im, pad);
            }
        }
    }
}

void im2col_cpu_cw(float* data_im, float* data_im_pad,
	int channels, int height, int width,
	int ksize, int stride, int pad, float* data_col)
{
	int c, h, w;
	int new_height = height + 2 * pad;
	int new_width = width + 2 * pad;
	//float new_data_im_temp[368640]; //IPV368640  APF122880
	//float *new_data_im = (float *)malloc(new_height*new_width*channels*sizeof(float));
	//memset(new_data_im, 0, sizeof(float)*new_width*new_height*channels);
	
	//printf("max = %d\n", new_width*new_height*channels);
	int data_size = height*width;
	int new_data_size = new_height*new_width;

	


	for (c = 0; c < channels; ++c)
	{
		for (h = 0; h < height; ++h)
		{
			for (w = 0; w < width; ++w)
			{
				data_im_pad[(h + pad)*new_width + w + pad + c*new_data_size] = data_im[h*width + w + c*data_size];//��ԭͼ�����һ��pad
				//new_data_im_temp[(h + pad)*new_width + w + pad + c*new_data_size] = *(data_im+h*width + w + c*data_size);
			}
		}
	}

	int channels_col = channels * ksize * ksize;

	int height_col = (height + 2 * pad - ksize) / stride + 1;
	int width_col = (width + 2 * pad - ksize) / stride + 1;
	//int col_size = height_col*width_col;

	int n = 0;
	for (c = 0; c < channels_col; ++c)
	{
		int w_offset = c % ksize;
		int h_offset = (c / ksize) % ksize;
		int c_im = c / ksize / ksize;
		for (h = 0; h < height_col; ++h)
		{
			for (w = 0; w < width_col; ++w)
			{
				int im_row = h_offset + h*stride;
				int im_col = w_offset + w*stride;
				//int col_index = c * col_size + h * width_col + w;
				int col_index = (c * height_col + h) * width_col + w;
				data_col[col_index] = data_im_pad[im_col + new_width*im_row + c_im*new_data_size];
				//*(data_col + col_index) = new_data_im_temp[im_col + new_width*im_row + c_im*new_data_size];
			}
		}
	}

	//free(new_data_im);
}