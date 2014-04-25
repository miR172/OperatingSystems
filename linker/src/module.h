/*
** 	Header for Module Structure
** 
** Each Module as it is in a module in executable file
** Implementation of functions is in module.c
** 	
**	By Qinhua Yan @ NYU Feb 2014
*/

#ifndef MODULE
#define MODULE

#include "utlist.h"
#include "variable.h"
#include "instruction.h"
#include "error.h"

typedef struct __STRUCT_Module{
	int index;	// index from 1
	int start;	// module's address
	int codeCount;	// # instructions
	int defCount;	// # defined symbols
	int useCount;	// # used symbols
	Var *defList;	// defined symbol table 
	Var *useList;	// refered symbol table 
	Error *wList;	// list of warnings (pointer to head)
	Instruction *instructionList;	// instructions table
	struct __STRUCT_Module *prev, *next;
} Module;

void iniModule(Module *);
/*initialize*/

void setModule(Module *);
/* resolve define table address */

void setInstruction(Module *, Var *, int);
/* resolve instruction address 
** call after the define list is solved 
** will call checkModule at the end */


void checkModule(Module *);
/* check use table of a module for non-use variables */


#endif

