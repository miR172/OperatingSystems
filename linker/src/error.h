/*
** 	Header for Error Structure
** 
** Each error object is an Error or Warning defined by rules on assignment
** Important difference between Error and Warning:
**   - Errors go with the variable/instruction,
**	So each variable/instruction has a pointer to an error
**	Thus errors are stored as a lonely node, 
**	only connected to the variable that raises it
**   - Warnings are listed after or before section of output,
**	So they are stored as linked nodes before printing
**
** Implementation of functions is in error.c
** 	By Qinhua Yan @ NYU Feb 2014
*/

#ifndef ERROR_H
#define ERROR_H

#include "utlist.h"

int eCount;
int wCount;

typedef struct __STRUCT_Error{
	char msg[256];	//message to print
	struct __STRUCT_Error *next;	
} Error;


void syntaxError(int,int);
/* handel sytax error when parsing, will cause exit of program*/

Error* iniError(char *);
/* constructor 
   initialize an error with given msg and return the pointer*/

Error* iniWarning(char *);
/* constructor
   initialize a warning with given msg and return the pointer */

void printError(Error *);
/* pointer of an error is passed in order to print it */

void printWarning(Error *);
/* the HEAD of a list of warnings will be passed */

#endif

