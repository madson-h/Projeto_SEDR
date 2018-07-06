
#include <stdio.h>
#include <opencv2/core/core.hpp>
#include <hls_opencv.h>
#include "header.h"

char outImage[IMG_HEIGHT_OR_ROWS][IMG_WIDTH_OR_COLS];
char outImageRef[IMG_HEIGHT_OR_ROWS][IMG_WIDTH_OR_COLS];

#define INPUT_IMAGE_CORE "c:\\Users\\madso\\Desktop\\circulo4.bmp"
#define INPUT_IMAGE_CORE_LS "c:\\Users\\madso\\Desktop\\pulmaooito.png"
#define OUTPUT_IMAGE_CORE "c:\\Users\\madso\\Desktop\\ProjetoResultado.bmp"

char kernel[KERNEL_DIM*KERNEL_DIM] = {
        0, 1, 0,
         1, 1, 1,
        0, 1, 0,
};

int main(){

    for(int l = 0;l<150;l++){
    int c0=0;
    int c1=0;
    int p=0;
    int n=0;

    // Lendo Círculo------------------------------------------------------------------------------------------------------------------------------
    printf("Load image %s\n",INPUT_IMAGE_CORE);
    cv::Mat imageSrc;
    imageSrc = cv::imread(INPUT_IMAGE_CORE);

    cv::cvtColor(imageSrc, imageSrc, CV_BGR2GRAY);
    printf("Image Rows:%d Cols:%d\n",imageSrc.rows, imageSrc.cols);

    // Lendo a Lena ------------------------------------------------------------------------------------------------------------------------------
    printf("Load image %s\n",INPUT_IMAGE_CORE_LS);
    cv::Mat imageSrcLS;
    imageSrcLS = cv::imread(INPUT_IMAGE_CORE_LS);

    cv::cvtColor(imageSrcLS, imageSrcLS, CV_BGR2GRAY);


    // Define streams for input and output
    hls::stream<uint_8_side_channel> inputStream;
    hls::stream<uint_8_side_channel> inputStream2;
    hls::stream<uint_8_side_channel> inputStream3;
    hls::stream<uint_8_side_channel> inputStream4;
    hls::stream<int_8_side_channel> outputStream;

    // OpenCV mat that point to a array (cv::Size(Width, Height))
    cv::Mat imgCvOut(cv::Size(imageSrc.cols, imageSrc.rows), CV_8UC1, outImage, cv::Mat::AUTO_STEP);

    // Populate the input stream with the image bytes
    for (int idxRows=0; idxRows < imageSrc.rows; idxRows++)
    {
        for (int idxCols=0; idxCols < imageSrc.cols; idxCols++)
        {
            //Pixels do circulo
            uint_8_side_channel valIn;
            valIn.data = imageSrc.at<unsigned char>(idxRows,idxCols);
            valIn.keep = 1; valIn.strb = 1; valIn.user = 1; valIn.id = 0; valIn.dest = 0;
            inputStream << valIn;

            //Pixels da Lena
            uint_8_side_channel valInLS;
            valInLS.data = imageSrcLS.at<unsigned char>(idxRows,idxCols);
            valInLS.keep = 1; valInLS.strb = 1; valInLS.user = 1; valInLS.id = 0; valInLS.dest = 0;
            inputStream2 << valInLS;

            //Pixels da Lena
            uint_8_side_channel valInLS3;
            valInLS3.data = imageSrcLS.at<unsigned char>(idxRows,idxCols);
            valInLS3.keep = 1; valInLS3.strb = 1; valInLS3.user = 1; valInLS3.id = 0; valInLS3.dest = 0;
            inputStream3 << valInLS;

            if(valIn.data ==0){
                c0 = c0 +(int)valInLS.data;
            }
            if(valIn.data ==1){
                c1=c1+(int)valInLS.data;
                n=n+1;
            }
            if(valIn.data ==0){
                p = p+1;
            }
        }
    }
    printf("c0=%d numero de amostras= %d e sua media= %d \n",c0,p,c0/p);
    printf("c1=%d numero de amostras= %d e sua media: %d \n",c1,n,c1/n);
    c0=c0/p;
    c1=c1/n;

        Boundary(inputStream, outputStream, inputStream2, kernel,c0,c1,l+1);

        printf("saindo da primeira funcao\n");

    //hls::stream<int_8_side_channel> outputStream2;

    //Boundary(inputStream4,outputStream2,inputStream3,kernel,c0,c1,3);

        for (int idxRows=0; idxRows < imageSrc.rows; idxRows++){
                for (int idxCols=0; idxCols < imageSrc.cols; idxCols++)
                {
                    int_8_side_channel valOut;
                    outputStream.read(valOut);
                    outImage[idxRows][idxCols] = valOut.data;
                }
            }
    char nome [10];
    char ext[5]=".bmp";
    sprintf(nome, "%d%s",l,ext);
    // Save image out file or display
        if (imageSrc.rows > 12){
            printf("Saving image\n");
            saveImage(std::string(nome) ,imgCvOut);
        }
    }
    return 0;
}
