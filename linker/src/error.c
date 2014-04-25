/*
** 	Implementation for Error Structure
**
**  Each error object is either an Error or a Warning 
**  Declarations in error.h
** 		
**		By Qinhua Yan @ NYU Feb 2014
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "utlist.h"
#include "error.h"

int eCount = 0;	//count along with linking process
int wCount = 0; //not required but may be useful with further functions of the linker

char* syntaxE[] = 
{
        "NUM_EXPECTED",         // 0
        "SYM_EXPECTED",         // 1
        "ADDR_EXPECTED",        // 2
        "SYM_TOLONG",           // 3
        "TO_MANY_DEF_IN_MODULE",// 4
        "TO_MANY_USE_IN_MODULE",// 5
};
//table for types of syntaxError


Error* iniError(char *msg){
	eCount++;
	Error *e = malloc(sizeof(Error));
	strcpy(e->msg,msg);
	return e;
}

Error* iniWarning(char *msg){
	wCount++;
	Error *w = malloc(sizeof(Error));
	strcpy(w->msg, msg);
	return w;
}


void syntaxError(int eType, int location){
/* raise error in parse() @ program.c */
/* will force program exit if called */
	printf("SyntaxError %s: Token@%d\nExit at parse().\n",syntaxE[eType], location);
	exit(0);	
}

void printError(Error *e){
	if(e==NULL){
		printf("\n");
		return;
	}
	printf(" Error: %s\n", e->msg);
	return;
}

void printWarning(Error *w){
	if(w==NULL){
		return;	
	}
	Error *t1,*t2;
	LL_FOREACH_SAFE(w, t1,t2){
		printf("Warning: %s\n", t1->msg);
		LL_DELETE(w,t1);
	}
}
