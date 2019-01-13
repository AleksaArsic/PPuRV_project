/*
 * ImageProcessing.c
 *
 *  Created on: Oct 20, 2018
 *      Author: aleksa
 */

#include "ImageProcessing.h"

int32_t MATRIX_WIDTH = 16;
int32_t MATRIX_HEIGHT = 16;

int32_t MATRIX_X = 0;
int32_t MATRIX_Y = 0;

uint8_t FIND_VALUE = 1;

/*
 * Ucitava sliku, pretvara je u binarnu matricu i kao takvu je vraca kao povratnu vrednost
 */
uint8_t* pixelsToBoolean (char* filename)
{

	uint8_t* outputData = (uint8_t*) malloc(MATRIX_WIDTH * MATRIX_HEIGHT);
	uint32_t i;
	uint32_t j;
	uint32_t m = 0;


	BITMAPFILEHEADER bmapFileHeader;
	BITMAPINFOHEADER bmapInfoHeader;
	uint8_t* bitmapData;

	/* MISRA-C:2004 17.4/R) Array indexing shall be the only allowed form of pointer arithmetic
	 * ne vidim u cemu je problem
	 */
	outputData[0] = 0;

	if (checkFileExtension(filename, BMP_E) == 0)
	{
		outputData = NULL;
		return outputData;
	}


	/* Loading bitmap*/
	bitmapData = (uint8_t*) malloc(MATRIX_WIDTH * MATRIX_HEIGHT);
	bitmapData = LoadBitmapFile(filename, &bmapFileHeader, &bmapInfoHeader);

	if (!bitmapData)
	{
		outputData = NULL;
		return outputData;
	}

	printf("Bitmap %s loaded.\n\n", filename);

	for (i = 0; i < (MATRIX_WIDTH * MATRIX_HEIGHT * 3); i+=3)
	{

		for (j = 0; j < 3; j++)
		{

			outputData[m] += bitmapData[j + i];
		}
		m++;
	}

	for (i = 0; i < (MATRIX_WIDTH * MATRIX_HEIGHT); i++)
	{
		if (outputData[i] >= 127)
		{
			outputData[i] = 49;
		}
		else
		{
			outputData[i] = 48;
		}
	}

	free(bitmapData);

	return outputData;
}

/*
 * Ucitava .txt fajl i vraca binarnu matricu kao povratnu vrednost
 */
uint8_t* readMatrixFromFile (char* filename)
{

	FILE* fp = NULL;

	uint8_t* retMatrix = (uint8_t*) malloc(MATRIX_WIDTH * MATRIX_HEIGHT);

	char temp[MATRIX_WIDTH + 2];
	int32_t i;
	int32_t j;

	retMatrix[0] = 0;
	temp[0] = 0;

	if (!checkFileExtension(filename, TXT_E))
	{
		retMatrix = NULL;
		return retMatrix;
	}

	fp = fopen((char*)filename, "r");

	if (fp == NULL)
	{
		retMatrix = NULL;
		return retMatrix;
	}

	j = 0;
	while ((fgets(temp, MATRIX_WIDTH + 2, fp)) != NULL)
	{
			for (i = 0; i < MATRIX_WIDTH; i++){
			if (*temp == '\n' || *temp == 0)
			{
				break;
			}
			if (temp[i] == 48 || temp[i] == 49)
			{
				retMatrix[j] = temp[i];
				j++;
			}
		}


	}

	fclose(fp);

	return retMatrix;
}

/*
 * Stampa binarnu matricu na konzolu
 */
void dumpMatrix(uint8_t* matrix){

	int32_t i;

	printf("*********************** MATRIX DUMP: ***********************");

	for (i = 0; i < (MATRIX_WIDTH * MATRIX_HEIGHT); i++)
	{
		if ((i % MATRIX_WIDTH) == 0)
		{
			printf("\n");
		}
		printf("%c ", matrix[i]);
	}

	printf("\n");
}

/*
 * Proverava da li je ekstenzija fajla uredu
 * Upotreba moguce neprenosivog tipa bool.
 */
bool checkFileExtension (char* filename, char* extension)
{

	bool retVal = true;
	int32_t i;
	int32_t j;

	i = 2;
	j = (int32_t) strlen(filename) - 1;
	char fileExtension[3];
	while (filename[j] != '.')
	{
		fileExtension[i] = filename[j];
		i--;
		j--;
	}

	if (strcmp(fileExtension, extension))
	{
		retVal = false;
		return retVal;
	}

	return retVal;
}

/*
 * Parsira dimenzije matrica iz argumenata komandne linije
 * Upotreba moguce neprenosivog tipa bool.
 */
bool matrixDimensionsParser (char* iWidth, char* iHeight)
{

	bool retVal = true;

	int32_t tempW = (int32_t) strtol(iWidth, NULL, 10);
	int32_t tempH = (int32_t) strtol(iHeight, NULL, 10);

	if (tempW <= 0 || tempH <= 0)
	{
		retVal = false;
		return retVal;
	}

	MATRIX_WIDTH = tempW;
	MATRIX_HEIGHT = tempH;

	return retVal;			//tempVal[m] = matrix[tempCoord];

}

/*
 * Parsira pocetne koordinate iz argumenata komandne linije
 * Upotreba moguce neprenosivog tipa bool.
 */
bool getCoordinates (const char* X, const char* Y)
{
	bool retVal = true;

	MATRIX_X = (int32_t) strtol(X, NULL, 10);
	MATRIX_Y = (int32_t) strtol(Y, NULL, 10);

	if ((MATRIX_X > MATRIX_WIDTH || MATRIX_X <= 0)
			|| (MATRIX_Y > MATRIX_HEIGHT || MATRIX_Y <= 0))
	{
		retVal = false;
		return retVal;
	}

	return retVal;
}

/*
 * Logika programa. Pronalazi istovrednosne susede i smesta ih na stek.
 */
void findNeighbours (const uint8_t* matrix)
{
	/* Variables for determining neighbours */
	int32_t i;
	int32_t j;
	int32_t m;
	int32_t tempCoord;

	/* Stack variables */
	struct node* stackTop;
	struct s_data foundNeighbour;

	struct node* subStack;
	struct s_data subNeighbour;
	int32_t depthCounter;

	/* Shared library function pointers */
	void* handle;
	void (*initStack)(struct node**);
	struct node* (*push)(struct node*, struct s_data);
	struct node* (*pop)(struct node*, struct s_data*);
	void (*dumpStack)(struct node*);
	bool (*isEmpty)(struct node*);
	char* error;

	FIND_VALUE = matrix[MATRIX_WIDTH * (MATRIX_Y - 1) + (MATRIX_X - 1)];
	printf("Value for which we are searching for equal neighbours: %c\n", FIND_VALUE);

	/* initialization of shared library functions */
	handle = dlopen("libstack_implementation.so", RTLD_LAZY);
	if (handle == NULL)
	{
		fputs(dlerror(), stderr);
		exit(1);
	}

	initStack = dlsym(handle, "initStack");
	push = dlsym(handle, "pushToStack");
	pop = dlsym(handle, "popFromStack");
	dumpStack = dlsym(handle, "dumpStack");
	isEmpty = dlsym(handle, "isStackEmpty");

	if ((error = dlerror()) != NULL)
	{
		fputs(error, stderr);
		exit(1);
	}

	/* Initialize stack top*/
	(*initStack)(&stackTop);

	/* Search for the same neighbours of starting point and push them to stack */
	searchNeighbours(&stackTop, matrix, MATRIX_X, MATRIX_Y);

	/* Demonstration of dumpStack function */
#if 1
	printf("\nCoordinates of the neighbours with the same value as a starting point: \n");
	(*dumpStack)(stackTop);
#endif

	writeResult(stackTop);

	(*initStack)(&subStack);

	depthCounter = 0;
	while (!isEmpty(stackTop) )
	{
		stackTop = (*pop)(stackTop, &subNeighbour);

		printf("\nStarting position: (%d, %d)\n", subNeighbour.x_coord + 1, subNeighbour.y_coord + 1);

		searchNeighbours(&subStack, matrix, subNeighbour.x_coord + 1, subNeighbour.y_coord + 1);

		depthCounter++;

	}

	writeResult(subStack);

	free(subStack);
	free(stackTop);
}

/*
 * Trazi istovrednosne komsije.
 */
void searchNeighbours(struct node** stackTop, const uint8_t* matrix, int32_t x_c, int32_t y_c)
{

	/* Variables for determining neighbours */
	int32_t i;
	int32_t j;
	int32_t m;
	int32_t tempCoord;

	/* Stack variables */
	struct s_data foundNeighbour;

	/* Shared library function pointers */
	void* handle;
	void (*initStack)(struct node**);
	struct node* (*push)(struct node*, struct s_data);
	struct node* (*pop)(struct node*, struct s_data*);
	void (*dumpStack)(struct node*);
	bool (*isEmpty)(struct node*);
	char* error;

	/* initialization of shared library functions */
	handle = dlopen("libstack_implementation.so", RTLD_LAZY);
	if (handle == NULL)
	{
		fputs(dlerror(), stderr);
		exit(1);
	}

	initStack = dlsym(handle, "initStack");
	push = dlsym(handle, "pushToStack");
	pop = dlsym(handle, "popFromStack");
	dumpStack = dlsym(handle, "dumpStack");
	isEmpty = dlsym(handle, "isStackEmpty");

	if ((error = dlerror()) != NULL)
	{
		fputs(error, stderr);
		exit(1);
	}

	m = 0;
	for (i = 2; i >= 0; i--)
	{
		for (j = 2; j >= 0; j--, m++)
		{
			/* Check if the coordinates are within bounds of a matrix */
			if ( (y_c - i) < 0 ||  (x_c - j) < 0
					|| (y_c - i) >= MATRIX_HEIGHT || (x_c - j) >= MATRIX_WIDTH)
			{
				continue;
			}

			tempCoord = MATRIX_WIDTH * (y_c - i) + (x_c - j);

			if (matrix[tempCoord] == FIND_VALUE)
			{
				/* Check if coordinates are not starting point coordinates.
				 * If they are not, push them to stack.
				 */

				if( ( (y_c - i) != (y_c - 1) )
						|| ( (x_c - j) != (x_c - 1) ) )
				{
					foundNeighbour.y_coord = (y_c - i);
					foundNeighbour.x_coord = (x_c - j);
					*stackTop = (*push)(*stackTop, foundNeighbour);
				}

			}
		}
	}
}

/*
 * Pise koordinate istovrednosnih suseda u fajl result.txt
 */
void writeResult (struct node* stackTop)
{
	FILE* fp;
	struct node* tmp = stackTop;


	fp = fopen("result.txt", "a");

	if (fp == NULL)
	{
		printf("Could not open/create output file.");
	}

	while (tmp != NULL)
	{
		fprintf(fp, "(%d, %d)\n", (tmp->data.x_coord + 1), (tmp->data.y_coord + 1));
		tmp = tmp->next;
	}

	fprintf(fp, "\n");
	fclose(fp);
}

