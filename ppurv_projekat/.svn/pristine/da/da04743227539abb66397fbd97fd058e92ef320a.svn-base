/*
 * bmp.c
 *
 *  Created on: Jan 11, 2011
 *      Author: romans
 */

#include "bmp.h"


BYTE* LoadBitmapFile(char* filename,
	BITMAPFILEHEADER* bitmapFileHeader,
	BITMAPINFOHEADER* bitmapInfoHeader) 
{

	FILE* filePtr; //our file pointer
	BYTE* bitmapImage; //store image data
	LONG imageIdx = 0; //image index counter
	BYTE tempRGB; //our swap variable

	//open filename in read binary mode
	filePtr = fopen(filename, "rb");
	if (filePtr == NULL)
		return NULL;

	//read the bitmap file header
	fread(bitmapFileHeader, sizeof(BITMAPFILEHEADER), 1, filePtr);

	//verify that this is a bmp file by check bitmap id
	if (bitmapFileHeader->bfType != 0x4D42) 
	{
		fclose(filePtr);
		return NULL;
	}

	//read the bitmap info header
	fread(bitmapInfoHeader, sizeof(BITMAPINFOHEADER), 1, filePtr);

	//move file point to the begging of bitmap data
	fseek(filePtr, bitmapFileHeader->bfOffBits, SEEK_SET);

	//allocate enough memory for the bitmap image data
	bitmapImage = (BYTE*) malloc(bitmapInfoHeader->biSizeImage);

	//verify memory allocation
	if (!bitmapImage) 
	{
		free(bitmapImage);
		fclose(filePtr);
		return NULL;
	}

	//read in the bitmap image data
	fread(bitmapImage, bitmapInfoHeader->biSizeImage, 1, filePtr);

	//make sure bitmap image data was read
	if (bitmapImage == NULL) 
	{
		fclose(filePtr);
		return NULL;
	}

	//swap the r and b values to get RGB (bitmap is BGR)
	for (imageIdx = 0; imageIdx < bitmapInfoHeader->biSizeImage; imageIdx+=3)
	{
		tempRGB = bitmapImage[imageIdx];
		bitmapImage[imageIdx] = bitmapImage[imageIdx + 2];
		bitmapImage[imageIdx + 2] = tempRGB;
	}

	//close file and return bitmap iamge data
	fclose(filePtr);
	return bitmapImage;
}


LONG SaveBitmapFile(char* filename, BITMAPFILEHEADER *bitmapFileHeader,
	BITMAPINFOHEADER *bitmapInfoHeader, BYTE *bitmapImage) 
{
	FILE* outFile;
	LONG imageIdx;
	BYTE tempRGB; //our swap variable

	//open filename in write binary mode
	outFile = fopen(filename, "wb");
	if (outFile == NULL)
		return -1;

	//write bitmap file header data
	fwrite(bitmapFileHeader, sizeof(BITMAPFILEHEADER), 1, outFile);

	//write bitmap info header data
	fwrite(bitmapInfoHeader, sizeof(BITMAPINFOHEADER), 1, outFile);

	//swap the r and b values to get RGB (bitmap is BGR)
	for (imageIdx = 0; imageIdx < bitmapInfoHeader->biSizeImage; imageIdx+=3)
	{
		tempRGB = bitmapImage[imageIdx];
		bitmapImage[imageIdx] = bitmapImage[imageIdx + 2];
		bitmapImage[imageIdx + 2] = tempRGB;
	}
	//write pixel data to file
	fwrite(bitmapImage, bitmapInfoHeader->biSizeImage, 1, outFile);

	fclose(outFile);
	return 0;
}

LONG bitmapLinePadding(BITMAPINFOHEADER *bitmapInfoHeader)
{
	return (bitmapInfoHeader->biSizeImage - bitmapInfoHeader->biHeight
		* bitmapInfoHeader->biWidth * 3) / bitmapInfoHeader->biHeight;
}
