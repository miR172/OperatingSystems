/*
** 	Header for Structure: Instruction 
** 
** Each instruction as it is in a reclocatable file (an input)
** 	consists of:
**		Type: I R E A
**		4digts code: 1 op 3 address except I-4digits op code
** Implementation in instruction.c
** 	
**	By Qinhua Yan @NYU 2014 Feb
*/

#ifndef INSTRUCTION
#define INSTRUCTION

#include "utlist.h"
#include "error.h"

typedef struct __STRUCT_Instruction{

	char type[2];	// instruction type [IARE] 
	int op;		// operation code 
	int addr;	// address 
	int relocated;	// flag relocated (will be mark to 1)
	Error *errorID;	// linked to an error if happened or NULL
	struct __STRUCT_Instruction *prev, *next;	// pointers to other instruction node
} Instruction;

void iniInstruction(Instruction *);
/* initialize */

void printCode(Instruction *, int);
/* print 4 digit instruction code in desired format 
** with error (if exists) */


#endif

