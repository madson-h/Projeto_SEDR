#include "header.h"

void Boundary(hls::stream<uint_8_side_channel> &inStream, hls::stream<int_8_side_channel> &outStream, hls::stream<uint_8_side_channel> &inStreamLS, char kernel[KERNEL_DIM*KERNEL_DIM], int c0,int c1, int iteracoes){

    int c2=0; int c3=0;
    int m=0;  int n=0; int limite = 0;

    hls::LineBuffer<KERNEL_DIM,IMG_WIDTH_OR_COLS,unsigned char> lineBuff;
    hls::Window<KERNEL_DIM,KERNEL_DIM,short> window;

    int idxRow=0;int idxCol = 0;
    int pixProcessed=0;

    int_8_side_channel dataOutSideChannel;
    uint_8_side_channel currPixelSideChannel;
    uint_8_side_channel currPixelSideChannelLS;

    unsigned char matrizLS [262144];
    //unsigned char matriz2 [262144];
    //unsigned char matriz [262144];
    ap_uint<1> matriz [262144];
    ap_uint<1> matriz2 [262144];

    unsigned char pixelIn;
    unsigned char pixelInLS;

    for(int idxPixell = 0; idxPixell < (IMG_WIDTH_OR_COLS*IMG_HEIGHT_OR_ROWS); idxPixell++){

        currPixelSideChannel = inStream.read();
        //pixelIn = currPixelSideChannel.data;

        currPixelSideChannelLS = inStreamLS.read();
        //pixelInLS = currPixelSideChannelLS.data;

        matrizLS [idxPixell] = currPixelSideChannelLS.data;
        matriz [idxPixell] = currPixelSideChannel.data;
        matriz2 [idxPixell] = currPixelSideChannel.data;

    }
for(int z = 0; z< iteracoes; z++){        //iteracoes
    c2=0;c3=0;m=0;n=0;
        for(int idxPixel = 0; idxPixel < (IMG_WIDTH_OR_COLS*IMG_HEIGHT_OR_ROWS); idxPixel++){

            /*lineBuff.shift_up(idxCol);

            if(z%2 == 0) lineBuff.insert_top(matriz[idxPixel],idxCol);
            if(z%2 != 0) lineBuff.insert_top(matriz2[idxPixel],idxCol);

            for(int idxWinRow = 0; idxWinRow<KERNEL_DIM; idxWinRow++){
                for(int idxWinCol = 0; idxWinCol<KERNEL_DIM; idxWinCol++){
                    short val = (short) lineBuff.getval(idxWinRow,idxWinCol+pixProcessed);
                    //val = (short) kernel[(idxWinRow*KERNEL_DIM)+idxWinCol]*val;

                    if(val != 0 && val != 1){
                        val = 0;
                    }

                    window.insert(val,idxWinRow,idxWinCol);
                    long valInWindow;
                    valInWindow = (long) window.getval(idxWinRow,idxWinCol);
                }
            }

            if(idxPixel < 512){
                short val = 0;
                window.insert(val,0,1);
                window.insert(val,1,1);
            }
            */

            short valOutput =0;
            //if((idxRow>=KERNEL_DIM-1)&&(idxCol>=KERNEL_DIM-1)){
            if(z%2 == 0){
                if(idxPixel <512 || idxPixel > 261120 /*|| idxPixel%512 == 0 || idxPixel%511 == 0*/){
                    matriz2[idxPixel] = 0;
                    //matriz[idxPixel] = 0;
                }
                else if(((matriz[idxPixel-512] - matriz[idxPixel+512])!=0) || ((matriz[idxPixel+1] - matriz[idxPixel-1])!=0)){
                //if(((window.getval(0,1)-window.getval(2,1)) != 0 ) || ((window.getval(1,0)-window.getval(1,2))!=0)){
                    limite=0;

                    limite =( (matrizLS[idxPixel]-c1)*(matrizLS[idxPixel]-c1)-(matrizLS[idxPixel]-c0)*(matrizLS[idxPixel]-c0) );
                    if(limite < 0) matriz2[idxPixel] = 1;
                    if(limite > 0) matriz2[idxPixel] = 0;

                }
            }

            if(z%2 != 0){
                if(idxPixel <512 || idxPixel > 261120 /*|| idxPixel%512 == 0 || idxPixel%511 == 0*/){
                    matriz[idxPixel] = 0;
                    //matriz2[idxPixel] = 0;
                }
                else if(((matriz2[idxPixel-512] - matriz2[idxPixel+512])!=0) || ((matriz2[idxPixel+1] - matriz2[idxPixel-1])!=0)){
                //if(((window.getval(0,1)-window.getval(2,1)) != 0 ) || ((window.getval(1,0)-window.getval(1,2))!=0)){
                limite=0;

                limite =( (matrizLS[idxPixel]-c1)*(matrizLS[idxPixel]-c1)-(matrizLS[idxPixel]-c0)*(matrizLS[idxPixel]-c0) );
                if(limite < 0) matriz[idxPixel] = 1;
                if(limite > 0) matriz[idxPixel] = 0;

                }
            }
                //else matriz[idxPixel] = matriz[idxPixel];

                //pixProcessed++;

            //if(idxCol < IMG_WIDTH_OR_COLS-1){
            //    idxCol++;
            //}
            //else{
            //    idxCol=0;
            //    idxRow++;
            //    pixProcessed=0;
            //}

            if(z%2 == 0){
                if(matriz2[idxPixel] == 0){
                    c2=c2+matrizLS[idxPixel];
                    m = m+1;
                }
                //else{
                if(matriz2[idxPixel] > 0){
                    c3 = c3+matrizLS[idxPixel];
                    n = n+1;
                }
            }

            if(z%2 != 0){
                if(matriz[idxPixel] == 0){
                    c2=c2+matrizLS[idxPixel];
                    m = m+1;
                }
                //else{
                if(matriz[idxPixel] > 0){
                    c3 = c3+matrizLS[idxPixel];
                    n = n+1;
                }
            }

            if(z == iteracoes -1){
                if(z%2 == 0){
                    dataOutSideChannel.data=matriz2[idxPixel];
                    dataOutSideChannel.keep=currPixelSideChannel.keep;
                    dataOutSideChannel.strb=currPixelSideChannel.strb;
                    dataOutSideChannel.user=currPixelSideChannel.user;
                    dataOutSideChannel.last=currPixelSideChannel.last;
                    dataOutSideChannel.dest=currPixelSideChannel.dest;

                    outStream.write(dataOutSideChannel);
                }
                if(z%2 != 0){
                        dataOutSideChannel.data=matriz[idxPixel];
                        dataOutSideChannel.keep=currPixelSideChannel.keep;
                        dataOutSideChannel.strb=currPixelSideChannel.strb;
                        dataOutSideChannel.user=currPixelSideChannel.user;
                        dataOutSideChannel.last=currPixelSideChannel.last;
                        dataOutSideChannel.dest=currPixelSideChannel.dest;

                        outStream.write(dataOutSideChannel);
                }
            }
        }

        if(z%2 == 0){
        	for(int o = 0; o<5;o++){
        		for(int idxPixell = 0; idxPixell < (IMG_WIDTH_OR_COLS*IMG_HEIGHT_OR_ROWS); idxPixell++){
        			matriz[idxPixell]=operacoemorfsi(matriz2[idxPixell],matriz2[idxPixell-512],matriz2[idxPixell+512],matriz2[idxPixell-1],matriz2[idxPixell+1],matriz2[idxPixell-511],matriz2[idxPixell-513],matriz2[idxPixell+513],matriz2[idxPixell+511]);
        		}
        		for(int idxPixell = 0; idxPixell < (IMG_WIDTH_OR_COLS*IMG_HEIGHT_OR_ROWS); idxPixell++){
        			matriz2[idxPixell]=operacoemorfis(matriz[idxPixell],matriz[idxPixell-512],matriz[idxPixell+512],matriz[idxPixell-1],matriz[idxPixell+1],matriz[idxPixell-511],matriz[idxPixell-513],matriz[idxPixell+513],matriz[idxPixell+511]);
        		}
        		for(int idxPixell = 0; idxPixell < (IMG_WIDTH_OR_COLS*IMG_HEIGHT_OR_ROWS); idxPixell++){
        			matriz[idxPixell]=operacoemorfsi(matriz2[idxPixell],matriz2[idxPixell-512],matriz2[idxPixell+512],matriz2[idxPixell-1],matriz2[idxPixell+1],matriz2[idxPixell-511],matriz2[idxPixell-513],matriz2[idxPixell+513],matriz2[idxPixell+511]);
        		}
        		for(int idxPixell = 0; idxPixell < (IMG_WIDTH_OR_COLS*IMG_HEIGHT_OR_ROWS); idxPixell++){
        			matriz2[idxPixell]=operacoemorfis(matriz[idxPixell],matriz[idxPixell-512],matriz[idxPixell+512],matriz[idxPixell-1],matriz[idxPixell+1],matriz[idxPixell-511],matriz[idxPixell-513],matriz[idxPixell+513],matriz[idxPixell+511]);
        		}
        	}

            for(int idxPixell = 0; idxPixell < (IMG_WIDTH_OR_COLS*IMG_HEIGHT_OR_ROWS); idxPixell++){
                matriz[idxPixell] = matriz2[idxPixell];
            }
        }

        if(z%2 != 0){
        	for(int o = 0; o<6;o++){
        		for(int idxPixell = 0; idxPixell < (IMG_WIDTH_OR_COLS*IMG_HEIGHT_OR_ROWS); idxPixell++){
        			matriz2[idxPixell]=operacoemorfsi(matriz[idxPixell],matriz[idxPixell-512],matriz[idxPixell+512],matriz[idxPixell-1],matriz[idxPixell+1],matriz[idxPixell-511],matriz[idxPixell-513],matriz[idxPixell+513],matriz[idxPixell+511]);
        		}
        		for(int idxPixell = 0; idxPixell < (IMG_WIDTH_OR_COLS*IMG_HEIGHT_OR_ROWS); idxPixell++){
        			matriz[idxPixell]=operacoemorfis(matriz2[idxPixell],matriz2[idxPixell-512],matriz2[idxPixell+512],matriz2[idxPixell-1],matriz2[idxPixell+1],matriz2[idxPixell-511],matriz2[idxPixell-513],matriz2[idxPixell+513],matriz2[idxPixell+511]);
        		}
        		for(int idxPixell = 0; idxPixell < (IMG_WIDTH_OR_COLS*IMG_HEIGHT_OR_ROWS); idxPixell++){
        			matriz2[idxPixell]=operacoemorfsi(matriz[idxPixell],matriz[idxPixell-512],matriz[idxPixell+512],matriz[idxPixell-1],matriz[idxPixell+1],matriz[idxPixell-511],matriz[idxPixell-513],matriz[idxPixell+513],matriz[idxPixell+511]);
        		}
        		for(int idxPixell = 0; idxPixell < (IMG_WIDTH_OR_COLS*IMG_HEIGHT_OR_ROWS); idxPixell++){
        			matriz[idxPixell]=operacoemorfis(matriz2[idxPixell],matriz2[idxPixell-512],matriz2[idxPixell+512],matriz2[idxPixell-1],matriz2[idxPixell+1],matriz2[idxPixell-511],matriz2[idxPixell-513],matriz2[idxPixell+513],matriz2[idxPixell+511]);
        		}
        	}
        	for(int idxPixell = 0; idxPixell < (IMG_WIDTH_OR_COLS*IMG_HEIGHT_OR_ROWS); idxPixell++){
        		matriz2[idxPixell] = matriz[idxPixell];
        	}
        }
        c0=c2/m;
        c1=c3/n;
        //printf("c0 = %ld/%ld = %ld   c1 = %ld/%ld = %ld \n",c2,m,c0,c3,n,c1);
    }
}
int min(int a, int b, int c){
	if( a<b){
		if(a<c) return a;
		else return c;
	}
	else{
		if(b<c) return b;
		else return c;
	}
}

int max(int a, int b, int c){
	if( a>b){
		if(a>c) return a;
		else return c;
	}
	else{
		if(b>c) return b;
		else return c;
	}
}

int maxsi(int a, int b, int c, int d){
	int l = max(a,b,c);
	if(d>l) return d;
	else return l;
}

int minis(int a, int b, int c, int d){
	int l = min(a,b,c);
	if(l<d) return l;
	else return d;
}

int operacoemorfsi(int pixel, int up, int down, int left, int right, int upright, int upleft, int downright, int downleft){
	int a = min(up,pixel,down);
	int b = min(left,pixel,right);
	int c = min(downleft,pixel,upright);
	int d = min(upleft,pixel,downright);

	int e = maxsi(a,b,c,d);
	return e;
}

int operacoemorfis(int pixel, int up, int down, int left, int right, int upright, int upleft, int downright, int downleft){
	int a = max(up,pixel,down);
	int b = max(left,pixel,right);
	int c = max(downleft,pixel,upright);
	int d = max(upleft,pixel,downright);

	int e = minis(a,b,c,d);
	return e;
}
