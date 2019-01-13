/*
 * ImageProcessing.h
 *
 *  Created on: Oct 20, 2018
 *  Author: aleksa
 */

#ifndef IMAGEPROCESSING_H_
#define IMAGEPROCESSING_H_

#ifdef __GNUC__

#include <dlfcn.h>

#endif

#include <inttypes.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include "bmp.h"
#include "StackImplementation.h"

/* Processor function calling depth */
#define PROC_DEPTH 4

/* File extensions */
#define TXT_E "txt"
#define BMP_E "bmp"

/* Global variables representing matrix dimensions */
extern int32_t MATRIX_WIDTH;
extern int32_t MATRIX_HEIGHT;

/* Global variables representing starting coordinates */
extern int32_t MATRIX_X;
extern int32_t MATRIX_Y;

/* Search value */
extern uint8_t FIND_VALUE;

uint8_t* pixelsToBoolean (char* filename);
uint8_t* readMatrixFromFile (char* filename);
void dumpMatrix(uint8_t* matrix);
bool checkFileExtension (char* filename, char* extension);
bool matrixDimensionsParser(char* iWidth, char* iHeight);
bool getCoordinates (const char* X, const char* Y);
void findNeighbours (const uint8_t* matrix);
void searchNeighbours(struct node** stackTop, const uint8_t* matrix, int32_t x_c, int32_t y_c);
void writeResult (struct node* stackTop);

#endif /* IMAGEPROCESSING_H_ */