/*
 * bmp.h
 *
 *  Created on: Jan 11, 2011
 *      Author: romans
 */

#ifndef BMP_H_
#define BMP_H_

#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>


#define BYTE  unsigned char
#define DWORD unsigned int
#define LONG  int
#define UINT  unsigned int
#define WORD  unsigned short int


typedef struct tagBITMAPFILEHEADER {
	WORD bfType; //specifies the file type
	DWORD bfSize; //specifies the size in bytes of the bitmap file
	WORD bfReserved1; //reserved; must be 0
	WORD bfReserved2; //reserved; must be 0
	DWORD bfOffBits; //species the offset in bytes from the bitmapfileheader to the bitmap bits
}__attribute__((packed)) BITMAPFILEHEADER;

typedef struct tagBITMAPINFOHEADER {
	DWORD biSize; //specifies the number of bytes required by the struct
	LONG biWidth; //specifies width in pixels
	LONG biHeight; //species height in pixels
	WORD biPlanes; //specifies the number of color planes, must be 1
	WORD biBitCount; //specifies the number of bit per pixel
	DWORD biCompression;//spcifies the type of compression
	DWORD biSizeImage; //size of image in bytes
	LONG biXPelsPerMeter; //number of pixels per meter in x axis
	LONG biYPelsPerMeter; //number of pixels per meter in y axis
	DWORD biClrUsed; //number of colors used by th ebitmap
	DWORD biClrImportant; //number of colors that are important
}__attribute__((packed)) BITMAPINFOHEADER;

typedef struct tagRGBQUAD {
	BYTE rgbBlue;
	BYTE rgbGreen;
	BYTE rgbRed;
	BYTE rgbReserved;
}__attribute__((packed)) RGBQUAD;


BYTE* LoadBitmapFile(char* filename, BITMAPFILEHEADER* bitmapFileHeader,
		BITMAPINFOHEADER* bitmapInfoHeader);

LONG SaveBitmapFile(char* filename, BITMAPFILEHEADER* bitmapFileHeader,
		BITMAPINFOHEADER* bitmapInfoHeader, BYTE* bitmapImage);

LONG bitmapLinePadding(BITMAPINFOHEADER* bitmapInfoHeader);

#endif /* BMP_H_ */
