#include "header.h"

void Boundary(hls::stream<uint_8_side_channel> &inStream, hls::stream<int_8_side_channel> &outStream, hls::stream<uint_8_side_channel> &inStreamLS, char kernel[KERNEL_DIM*KERNEL_DIM], int c0,int c1, int iteracoes){

    int c2=0; int c3=0;
    int m=0;  int n=0; int limite = 0;

    char *pmatriz;
    char *pmatriz2;

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
    if(z%2 == 0){
        pmatriz =(char*) matriz2;
        pmatriz2 =(char*) matriz;
    }
    else{
        pmatriz = (char*) matriz;
        pmatriz2 =(char*) matriz2;
    }

        //QUALQUER ERRO, TENTAR SEM O ÍNDICE DAS MATRIZES
        for(int idxPixel = 0; idxPixel < (IMG_WIDTH_OR_COLS*IMG_HEIGHT_OR_ROWS); idxPixel++){
            if(idxPixel <512 || idxPixel > 261120){
                pmatriz[idxPixel] = 0;
            }
            else if (((pmatriz2[idxPixel-512] - pmatriz2[idxPixel+512])!=0) || ((pmatriz2[idxPixel+1] - pmatriz2[idxPixel-1])!=0)){
                limite = 0;

                limite =((matrizLS[idxPixel]-c1)*(matrizLS[idxPixel]-c1)-(matrizLS[idxPixel]-c0)*(matrizLS[idxPixel]-c0));
                if(limite < 0) pmatriz[idxPixel] = 1;
                if(limite > 0) pmatriz[idxPixel] = 0;
            }

            //if((idxRow>=KERNEL_DIM-1)&&(idxCol>=KERNEL_DIM-1)){
            /*if(z%2 == 0){
                if(idxPixel <512 || idxPixel > 261120 /*|| idxPixel%512 == 0 || idxPixel%511 == 0*///){
            /*        matriz2[idxPixel] = 0;
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
                if(idxPixel <512 || idxPixel > 261120 /*|| idxPixel%512 == 0 || idxPixel%511 == 0){*/
           /*         matriz[idxPixel] = 0;
                    //matriz2[idxPixel] = 0;
                }
                else if(((matriz2[idxPixel-512] - matriz2[idxPixel+512])!=0) || ((matriz2[idxPixel+1] - matriz2[idxPixel-1])!=0)){
                //if(((window.getval(0,1)-window.getval(2,1)) != 0 ) || ((window.getval(1,0)-window.getval(1,2))!=0)){
                limite=0;

                limite =( (matrizLS[idxPixel]-c1)*(matrizLS[idxPixel]-c1)-(matrizLS[idxPixel]-c0)*(matrizLS[idxPixel]-c0) );
                if(limite < 0) matriz[idxPixel] = 1;
                if(limite > 0) matriz[idxPixel] = 0;

                }
            }*/

            if(pmatriz[idxPixel] == 0){
                c2=c2+matrizLS[idxPixel];
                m = m+1;
            }
                            //else{
            if(pmatriz[idxPixel] > 0){
                c3 = c3+matrizLS[idxPixel];
                n = n+1;
            }

            /*
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
            */
            if(z == iteracoes -1){

                dataOutSideChannel.data=pmatriz[idxPixel];
                dataOutSideChannel.keep=currPixelSideChannel.keep;
                dataOutSideChannel.strb=currPixelSideChannel.strb;
                dataOutSideChannel.user=currPixelSideChannel.user;
                dataOutSideChannel.last=currPixelSideChannel.last;
                dataOutSideChannel.dest=currPixelSideChannel.dest;

                outStream.write(dataOutSideChannel);
            }

            /*
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
            }*/
        }

        for(int o = 0; o<5;o++){
                for(int idxPixell = 0; idxPixell < (IMG_WIDTH_OR_COLS*IMG_HEIGHT_OR_ROWS); idxPixell++){
                    pmatriz2[idxPixell]=operacoemorfsi(pmatriz[idxPixell],pmatriz[idxPixell-512],pmatriz[idxPixell+512],pmatriz[idxPixell-1],pmatriz[idxPixell+1],pmatriz[idxPixell-511],pmatriz[idxPixell-513],pmatriz[idxPixell+513],pmatriz[idxPixell+511]);
                }
                for(int idxPixell = 0; idxPixell < (IMG_WIDTH_OR_COLS*IMG_HEIGHT_OR_ROWS); idxPixell++){
                    pmatriz[idxPixell]=operacoemorfis(pmatriz2[idxPixell],pmatriz2[idxPixell-512],pmatriz2[idxPixell+512],pmatriz2[idxPixell-1],pmatriz2[idxPixell+1],pmatriz2[idxPixell-511],pmatriz2[idxPixell-513],pmatriz2[idxPixell+513],pmatriz2[idxPixell+511]);
                }
                for(int idxPixell = 0; idxPixell < (IMG_WIDTH_OR_COLS*IMG_HEIGHT_OR_ROWS); idxPixell++){
                    pmatriz2[idxPixell]=operacoemorfsi(pmatriz[idxPixell],pmatriz[idxPixell-512],pmatriz[idxPixell+512],pmatriz[idxPixell-1],pmatriz[idxPixell+1],pmatriz[idxPixell-511],pmatriz[idxPixell-513],pmatriz[idxPixell+513],pmatriz[idxPixell+511]);
                }
                for(int idxPixell = 0; idxPixell < (IMG_WIDTH_OR_COLS*IMG_HEIGHT_OR_ROWS); idxPixell++){
                    pmatriz[idxPixell]=operacoemorfis(pmatriz2[idxPixell],pmatriz2[idxPixell-512],pmatriz2[idxPixell+512],pmatriz2[idxPixell-1],pmatriz2[idxPixell+1],pmatriz2[idxPixell-511],pmatriz2[idxPixell-513],pmatriz2[idxPixell+513],pmatriz2[idxPixell+511]);
                }
            }

            for(int idxPixell = 0; idxPixell < (IMG_WIDTH_OR_COLS*IMG_HEIGHT_OR_ROWS); idxPixell++){
                pmatriz2[idxPixell] = pmatriz[idxPixell];
        }
        /*
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
        }*/
        c0=c2/m;
        c1=c3/n;
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

/*
#include "header.h"

void Boundary(hls::stream<uint_8_side_channel> &inStream, hls::stream<int_8_side_channel> &outStream, hls::stream<uint_8_side_channel> &inStreamLS, char kernel[KERNEL_DIM*KERNEL_DIM], int c0,int c1, int iteracoes){

    int c2=0; int c3=0;
    int m=0;  int n=0; int limite = 0;

    char *pmatriz;
    char *pmatriz2;

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
    if(z%2 == 0){
    	pmatriz =(char*) matriz2;
    	pmatriz2 =(char*) matriz;
    }
    else{
    	pmatriz = (char*) matriz;
    	pmatriz2 =(char*) matriz2;
    }

    	//QUALQUER ERRO, TENTAR SEM O ÍNDICE DAS MATRIZES
        for(int idxPixel = 0; idxPixel < (IMG_WIDTH_OR_COLS*IMG_HEIGHT_OR_ROWS); idxPixel++){
        	if(idxPixel <512 || idxPixel > 261120){
        		pmatriz = 0;
        	}
        	else if (((pmatriz2[idxPixel-512] - pmatriz2[idxPixel+512])!=0) || ((pmatriz2[idxPixel+1] - pmatriz2[idxPixel-1])!=0)){
        		limite = 0;

        		limite =((matrizLS[idxPixel]-c1)*(matrizLS[idxPixel]-c1)-(matrizLS[idxPixel]-c0)*(matrizLS[idxPixel]-c0));
        		if(limite < 0) pmatriz = 1;
        		if(limite > 0) pmatriz = 0;
        	}

        	//if((idxRow>=KERNEL_DIM-1)&&(idxCol>=KERNEL_DIM-1)){
            /*if(z%2 == 0){
                if(idxPixel <512 || idxPixel > 261120 /*|| idxPixel%512 == 0 || idxPixel%511 == 0*///){
            /*        matriz2[idxPixel] = 0;
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
                if(idxPixel <512 || idxPixel > 261120 /*|| idxPixel%512 == 0 || idxPixel%511 == 0){*/
           /*         matriz[idxPixel] = 0;
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

        	if(pmatriz == 0){
        		c2=c2+matrizLS[idxPixel];
        		m = m+1;
        	}
        	                //else{
        	if(pmatriz > 0){
        		c3 = c3+matrizLS[idxPixel];
        		n = n+1;
        	}

        	/*
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

        		dataOutSideChannel.data=pmatriz;
        		dataOutSideChannel.keep=currPixelSideChannel.keep;
        		dataOutSideChannel.strb=currPixelSideChannel.strb;
        		dataOutSideChannel.user=currPixelSideChannel.user;
        		dataOutSideChannel.last=currPixelSideChannel.last;
        		dataOutSideChannel.dest=currPixelSideChannel.dest;

        		outStream.write(dataOutSideChannel);
        	}

        	/*
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

		for(int o = 0; o<5;o++){
        		for(int idxPixell = 0; idxPixell < (IMG_WIDTH_OR_COLS*IMG_HEIGHT_OR_ROWS); idxPixell++){
        			pmatriz2=operacoemorfsi(pmatriz,pmatriz[idxPixell-512],pmatriz[idxPixell+512],pmatriz[idxPixell-1],pmatriz[idxPixell+1],pmatriz[idxPixell-511],pmatriz[idxPixell-513],pmatriz[idxPixell+513],pmatriz[idxPixell+511]);
        		}
        		for(int idxPixell = 0; idxPixell < (IMG_WIDTH_OR_COLS*IMG_HEIGHT_OR_ROWS); idxPixell++){
        			pmatriz=operacoemorfis(pmatriz2,pmatriz2[idxPixell-512],pmatriz2[idxPixell+512],pmatriz2[idxPixell-1],pmatriz2[idxPixell+1],pmatriz2[idxPixell-511],pmatriz2[idxPixell-513],pmatriz2[idxPixell+513],pmatriz2[idxPixell+511]);
        		}
        		for(int idxPixell = 0; idxPixell < (IMG_WIDTH_OR_COLS*IMG_HEIGHT_OR_ROWS); idxPixell++){
        			pmatriz2=operacoemorfsi(pmatriz,pmatriz[idxPixell-512],pmatriz[idxPixell+512],pmatriz[idxPixell-1],pmatriz[idxPixell+1],pmatriz[idxPixell-511],pmatriz[idxPixell-513],pmatriz[idxPixell+513],pmatriz[idxPixell+511]);
        		}
        		for(int idxPixell = 0; idxPixell < (IMG_WIDTH_OR_COLS*IMG_HEIGHT_OR_ROWS); idxPixell++){
        			pmatriz=operacoemorfis(pmatriz2,pmatriz2[idxPixell-512],pmatriz2[idxPixell+512],pmatriz2[idxPixell-1],pmatriz2[idxPixell+1],pmatriz2[idxPixell-511],pmatriz2[idxPixell-513],pmatriz2[idxPixell+513],pmatriz2[idxPixell+511]);
        		}
        	}

            for(int idxPixell = 0; idxPixell < (IMG_WIDTH_OR_COLS*IMG_HEIGHT_OR_ROWS); idxPixell++){
                pmatriz2 = pmatriz;
        }
        /*
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
        }*/
/*        c0=c2/m;
        c1=c3/n;
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
*/
