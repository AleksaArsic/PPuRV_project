/*
 * StackImplementation.c
 *
 *  Created on: Oct 19, 2018
 *      Author: aleksa
 */

#include "StackImplementation.h"
#include <stdio.h>

/*
 * Inicijalizuje vrednost pokazivaca na pokazivac steka na NULL
 */
void initStack (struct node** stackTop)
{
	*stackTop = NULL;
}

/*
 * Postavlja na vrh steka strukturu sa koordinatama i vraca pokazivac na
 * novi vrh steka
 */
struct node* pushToStack (struct node* stackTop, struct s_data iData)
{

	struct node* tmp = (struct node*) malloc(sizeof(struct node));
	if (tmp == NULL)
	{
		printf("Error. Not enough memory!\n");
		return tmp;
	}

	tmp->next = stackTop;
	tmp->data = iData;

	stackTop = tmp;

	return stackTop;
}

/*
 * Uklanja strukturu sa vrha steka i vraca je kao povratnu vrednost
 */
struct node* popFromStack (struct node* stackTop, struct s_data* oData)
{
	struct node* newTop = stackTop->next;
	*oData = stackTop->data;

	free(stackTop);

	return newTop;

}

/*
 * Proverava da li je stek prazan. U slucaju da jeste vraca true, u suprotnom false.
 * Upotreba moguce neprenosivog tipa bool.
 *
 */
bool isStackEmpty (struct node* stackTop)
{
	bool isEmpty = false;

	if (stackTop == NULL)
	{
		isEmpty = true;
		return isEmpty;
	}

	return isEmpty;
}

/*
 * Stampa stek na konzolu
 */
void dumpStack (struct node* stackTop)
{

	struct node* tmp = stackTop;

	while (tmp != NULL)
	{
		printf("(%d, %d)\n", (tmp->data.x_coord + 1), (tmp->data.y_coord + 1));
		tmp = tmp->next;
	}

}
