//API: 2
#ifndef __BMP_WRITER__
#define __BMP_WRITER__
#include<stdio.h>
#include<stdlib.h>
int _BMPX,_BMPY;
unsigned char *_BMP;

int putpixel(int x,int y,int r,int g,int b)
{
  int ry=_BMPY-y;
  int bo=ry*_BMPX*3+x*3-3;
  _BMP[bo]=b;
  _BMP[bo+1]=g;
  _BMP[bo+2]=r;
}

int bmpsave(const char *A)
{
    int width=_BMPX,height=_BMPY;
	unsigned char* I=_BMP;
    int row_width=((width*3+3)/4)*4,padding=row_width-width*3;
    long bs,filesize=row_width*height+54;
    bs=row_width*height;
    FILE* _BMPfp=fopen(A,"wb");
    unsigned char *_BMPI;
    _BMPI=(unsigned char*)malloc(filesize);
    if(!_BMPI)printf("BMP WRITER MEMORY ALLOCATION ERROR");
    int i;
    for(i=6;i<54;i++)
      _BMPI[i]=0;
    _BMPI[0]=66;_BMPI[1]=77;
    _BMPI[2]=filesize%256;filesize=filesize/256;
    _BMPI[3]=filesize%256;filesize=filesize/256;
    _BMPI[4]=filesize%256;filesize=filesize/256;
    _BMPI[5]=filesize%256;
    _BMPI[10]=54;_BMPI[14]=40;
    _BMPI[18]=width%256;
    _BMPI[19]=(width/256)%256;
    _BMPI[22]=height%256;
    _BMPI[23]=(height/256)%256;
    _BMPI[28]=24;
    _BMPI[34]=bs%256;bs/=256;
    _BMPI[35]=bs%256;bs/=256;
    _BMPI[36]=bs%256;bs/=256;
    _BMPI[37]=bs%256;
    _BMPI[38]=19;
    _BMPI[39]=11;
    _BMPI[42]=19;
    _BMPI[43]=11; 
    int x,y;
    for(y=0;y<height;y++)
    {
       for(x=0;x<width;x++)
       {
           _BMPI[y*row_width+x*3+54]=_BMP[y*width*3+x*3];
           _BMPI[y*row_width+x*3+55]=_BMP[y*width*3+x*3+1];
           _BMPI[y*row_width+x*3+56]=_BMP[y*width*3+x*3+2];
       }
       for(x=0;x<padding;x++)
           _BMPI[y*row_width+width*3+x+54]=x;
    }
    filesize=row_width*height+54;
    fwrite(_BMPI,filesize,1,_BMPfp);
    fclose(_BMPfp);
    return filesize;
}
void bmpinit(int X,int Y)
{
  _BMP=(unsigned char*)malloc(X*Y*3);
  if(!_BMP)printf("init ERROR");
  _BMPX=X,_BMPY=Y;
}
void bmpfree()
{
	free(_BMP);
}
#endif
