/*
** 	Header of Program Structure
** 
** One Program object for each input ( a relocatable file ) 
** Implementation in program.c
** 
**	By Qinhua Yan @ NYU Feb 2014
*/

#ifndef PROGRAM_H
#define PROGRAM_H

#include "utlist.h"
#include "module.h"
#include "variable.h"
#include "instruction.h"
#include "error.h"

typedef struct __STRUCT_Program{
	char *name;	//leave for further use (e.g could be file name when linking multiple programs)
	Module *moduleList;	// pointer to head of modules list		
	int varCount, moduleCount, instructionCount;
	Var *globalList;	// pointer to head of global table
	Instruction *instructions; // pointer to head of instruction table
	Error *wList;	// pointer to head of warnings list
}Program; 


void iniProgram();
/* initialize */

Program parse(); /* pass 1 */
/* parse object file and construct program stuct, return the struct*/

void solve(Program *);	/* pass 2 */
/* solve the define global table in each module
**	store them to program's table
** solve instructions in each module
**	store them to program's list
*/

void checkProgram(Program *);
/* check global variables for Non-use  warning and print them */

void print(Program *);
/* print the relocated program 
** will call printVar(), printCode() in variable.c and printCode.c
** and checkProgram() each of which calls
** printError() or printWarning() in error.c if necessary */

#endif //PROGRAM_H
