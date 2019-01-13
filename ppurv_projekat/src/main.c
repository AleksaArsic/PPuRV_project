/*
 ============================================================================
 Name        : main.c
 Author      : Aleksa Arsic RA119/2015
 Version     : 1.0
 Copyright   : Notice
 Description : PPuRV project 36 in C, Ansi-style
 ============================================================================
 */

/*
 * ZADATAK 36
 * Направити модул који представља стек. Модул извести као динамичку библиотеку (дељени
 * објекат). Написати програм који учитава слику из датотеке у форми матрице нула и јединица.
 * Затим написати функцију која креће од задатог пиксела и проналази све пикселе исте вредности
 * који су му суседни, као и све пикселе исте вредности који су суседи његовим истовредносним
 * суседима, итд. Узети у обзир да је на претпостављеном наменском процесору максимална дубина
 * позива функција 4. Дељени објекат који представља стек динамички повезати током извршавања.
 * У решењу се ослонити на структуру стека из модула.
 *
 * Potrebno je implementirati stek kao dinamicku biblioteku. Zatim ostvariti ucitavanje slike u formi binarne matrice
 * te za zadate pocetne koordinate odrediti istovrednosne susede, kao i istovrednosne susede tih suseda. Ako je na pocetnim
 * koordinatama 1 potrebno je pronaci susede koji su takodje 1, a ako je na pocetnim koordinatama 0 potrebno je pronaci
 * susede koji su 0. Procesor moze pozivati funkcije do maksimalne dubine 4. U resenju osloniti se na deljenu biblioteku koja
 * implementira stek.
 */

/*
 * OPIS RESENJA
 *
 * Stek je implementiran kao spregnuta lista u projektu stack_implementation.
 * Funkcionalnosti koje implementira StackImplementation.h su sledece:
 *
 * void initStack (NODE** stackTop);
 * NODE* pushToStack (NODE* stackTop, DATA iData);
 * NODE* popFromStack (NODE* stackTop, DATA* oData);
 * bool isStackEmpty (NODE* stackTop);
 * void dumpStack (NODE* stackTop);
 *
 * Opis funkcija pogledati u StackImplementation.c
 *
 * U projektu ppurv_projekat implementiran je drugi deo zadatka.
 * U program moguce je ucitati sliku u .bmp formatu, kao i tekstualni fajl koji sadrzi binarnu matricu.
 * Kada se radi o ucitavanju slike, implementacija se poziva na bmp.h zaglavlje koje je obelodanjeno u danu 4, vezba 4.1
 * predmeta PPuRV. Pomocu njega slika se ucitava u program kao bitmapa. Sva obrada vrsi se u ImageProcessing.h zaglavlju.
 * Bitmapa se pretvara u binarnu matricu pomocu funkcije uint8_t* pixelsToBoolean (char* filename);.
 * Ako u program ucitavamo .txt fajl funkcija uint8_t* readMatrixFromFile (char* filename); sluzi
 * za ucitavanje matrice u program.
 * Za detaljniji opis funkcija pogledati zaglavlje ImageProcessing.c.
 *
 * void findNeighbours (const uint8_t* matrix); vrsi pretragu po zadatoj kordinati koja se cita iz argumenata komandne linije
 * i na osnovu nje trazi istovrednosne susede.
 * Prva pretraga funkcionise. Dok kod narednih nesto nije uredu.
 * Pretraga funkcionise po principu da se provere sve susedne koordinate pazeci na granicne slucajeve (izlazenje van opsega niza)
 * Ako se nadje istovrednosni sused postavlja se na stek u vidu njegovih koordinata (x, y)
 *
 *
 * PRETPOSTAVKA: Poznavanje dimenzija ulazne matrice
 *
 */

/*
 * BUILD PROCES
 *	Uvezati dinamicku bibiloteku iz projekta stack_implementation
 */

/*
 * POKRETANJE PROGRAMA
 *
 * ARGUMENTS: IN_FILE OPTION WIDTH HEIGHT X_COORD Y_COORD
 *
 * IN_FILE: Input file
 * OPTION: Is it .bmp or .txt format of file
 * WIDTH: Matrix width
 * HEIGHT: Matrix height
 * X_COORD: Index of column for the starting point
 * Y_COORD: Index of row for the starting point
 *
 */

/*
 * MISRA C 2004 KRSENJE:
 * 5.7/A -> promenjljive se nalaze u razlicitim scope-ovima
 * 20.4/R -> malloc koristen zbog potreba implementacije (error: assignment to expression with array type error)
 * 17.4/R -> iako koristimo indeksiranje (npr. niz[3] = 22) izbacuje krsenje
 * 20.4/R -> kako bi se mogla proveriti funkcionalnost programa potrebno je npr. stapmati na izlaz (stdio.h)
 *
 */

#ifdef __GNUC__

#include <dlfcn.h>

#endif

#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
#include "StackImplementation.h"
#include "ImageProcessing.h"

#define __ARG_NUM__ 7
#define ARG_STRING "INPUT_FILE OPTION WIDTH HEIGHT X_COORD Y_COORD"


int main(int argc, char* argv[]) {

	/* Arguments variables*/
	int32_t option;

	/* Upotreba moguce neprenosivog tipa bool. */
	bool checkPoint;

	/* Function pointer array to ImageProcessing.h functions */
	uint8_t* (*proccesingPtr[2])(char*) = {pixelsToBoolean, readMatrixFromFile};

	/* Bitmap reader and file reader variables */
	uint8_t* booleanMatrix;

	if (argc != __ARG_NUM__)
	{
		printf("Enter valid arguments. \n");
		printf(ARG_STRING);
		return EXIT_FAILURE;
	}

	/* .txt or .bmp ? */
	option = (int32_t) strtol(argv[2], NULL, 10);

	/* Parse matrix dimensions */
	checkPoint = matrixDimensionsParser(argv[3], argv[4]);

	/* Check validity of dimensions */
	if (!checkPoint)
	{
		printf("Matrix dimensions can not be negative and should be greater than zero.");
		return EXIT_FAILURE;
	}

	/* Get coordinates from arguments */
	checkPoint = getCoordinates(argv[5], argv[6]);

	/* Check validity of coordinates */
	if (!checkPoint)
	{
		printf("Coordinates (%d, %d) are not valid.\n", MATRIX_X, MATRIX_Y);
		return EXIT_FAILURE;
	}

	/* pixels to boolean matrix*/
	booleanMatrix = (uint8_t*) malloc(MATRIX_WIDTH * MATRIX_HEIGHT);
	booleanMatrix = (*proccesingPtr[option])( argv[1]);

	if (booleanMatrix == NULL)
	{
		if (option == 0)
			printf("Error! Can't open input bitmap file: %s\n", argv[1]);
		if (option == 1)
			printf("Error! Can't open input text file: %s\n", argv[1]);
		return EXIT_FAILURE;
	}

	/* Dump matrix */
	dumpMatrix(booleanMatrix);

	printf("\nStarting position: (%d, %d)\n", MATRIX_X, MATRIX_Y);


	findNeighbours(booleanMatrix);


	free(booleanMatrix);

	return EXIT_SUCCESS;
}
