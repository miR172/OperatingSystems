/*
** 	Implementation of Structure: Instruction 
** 
** Each instruction object = an instruction in executable file
** Declaration in instruction.h
**	
** 	By Qinhua Yan @NYU 2014 Feb
*/


#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "utlist.h"
#include "instruction.h"
#include "error.h"

void iniInstruction(Instruction *i){
/* initialize */
	i->op	= 0;
	i->addr = 0;
	i->relocated = 0;
	i->errorID = NULL;
}

void printCode(Instruction *in, int type){
/* construct 4digits code from op and addr 
** print modified instruction */

	int i = in->op;
	int j = in->addr;
	int l = i*1000+j;
	
	if (type==0){
		printf("%d ",l);
	}
	else{
		printf("%s %d ",in->type,l);
	}
	
	printError(in->errorID);
	
	return;
}
