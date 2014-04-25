/*
** 		Main for Two-pass Linker
**
** Compile will other *.c for an executable C program: linker
** Required headers are:
** 	variable.h instruction.h module.h program.h error.h
**	utlist.h is a open source data structure macros for linked list
**		see utlist.h for copyright and more
** 
** To use the linker, and find more information, please see README
** 
**		All work except utlist.h:
** 		By Qinhua Yan @ NYU Feb 2014
**
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "utlist.h"
#include "variable.h"
#include "instruction.h"
#include "module.h"
#include "program.h"
#include "error.h"

int machineSize = 512;

int main(int argc, char *argv[]){
	if (argc != 2) {
		printf("usage: %s filename\n",argv[0]);
	}
	else{
		FILE *file = fopen(argv[1],"r");
		if (file ==0){
			printf("Cannot open file\n");
		}
		else{
		Program program;
		program = parse(file);	
		//parse input; function in program.c
		fclose(file);

		solve(&program);
		//solve & relocate; function in program.c
		print(&program);	
		//final error check&print; function in program.c	
		}
	}
	return 0;
}


