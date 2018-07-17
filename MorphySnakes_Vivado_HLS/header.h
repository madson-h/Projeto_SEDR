#include "hls_video.h"
//#include <ap_axi_sdata.h>
//#include <stdio.h>
//#include "core.hpp"
//#include <hls_opencv.h>

#define IMG_WIDTH_OR_COLS 512
#define IMG_HEIGHT_OR_ROWS 512
#define KERNEL_DIM 3

typedef ap_axiu<8,2,5,6> uint_8_side_channel;
typedef ap_axis<8,2,5,6> int_8_side_channel;

void Boundary(hls::stream<uint_8_side_channel> &inStream, hls::stream<int_8_side_channel> &outStream,  hls::stream<uint_8_side_channel> &inStreamLS, char kernel[KERNEL_DIM*KERNEL_DIM], int c0, int c1, int iteracoes);

int max(int a, int b, int c);

int min(int a, int b, int c);

int maxsi(int a, int b, int c, int d);

int minis(int a, int b, int c, int d);

int operacoemorfis(int pixel, int up, int down, int left, int right, int upright, int upleft, int downright, int downleft);

int operacoemorfsi(int pixel, int up, int down, int left, int right, int upright, int upleft, int downright, int downleft);


extern int c0;
extern int c1;
