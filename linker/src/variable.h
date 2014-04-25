/*
** 	Header for Variable Structure
** 
** Each variable object is one element in symbol tables
**	Tables are implemented by linked list, 
**	So each variable can be regarded as a node, 
**	with pointer to previous and next node.
** Implementation of functions is in variable.c
** 	
**	By Qinhua Yan @ NYU Feb 2014
*/

#ifndef VARIABLE
#define VARIABLE

#include "utlist.h"
#include "error.h"

typedef struct __STRUCT_Var{
	char name[17];	//name
	int addr;	//address
	int defined;	//will be set to module -> index
	int solved;	//flag for solved(will be set to 1)
	Error *errorID;	//link to an error if happend
	struct __STRUCT_Var *prev, *next; //pointer to other nodes
} Var;

void iniVar(Var *);
/*initialize*/

Var* findVar(Var *, Var *, int);
/* find var in a given list by name*/
/* return found var(pointer), if not found return NULL*/

void printVar(Var *);
/* print variable with desired format and error message*/

#endif

