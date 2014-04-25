/*
** 	Implementation for Variable Structure
**
** Each variable object is one element in variable tables 
**	(symbol tables); tables are implemented by linked lists
** Declaration is in variable.h
**
** 	By Qinhua Yan @ NYU Feb 2014
*/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "utlist.h"
#include "variable.h"
#include "error.h"

void iniVar(Var *v){
	v->addr = 0;
	v->defined = 0;
	v->solved = 0;	
	v->errorID = NULL;
	v->prev = NULL;
	v->next = NULL;
}

Var* findVar(Var *v, Var *list, int len){
	if (list ==NULL) { return NULL; }
	int i = 0;
	Var *t = list;
	while (i<len){
		if (strcmp(v->name,t->name)==0){
			return t;
		}
		i++;
		t = t->next;
	}
	return NULL;
}
	

void printVar(Var *v){
	printf("%s=%d ",v->name,v->addr);
	printError(v->errorID); 
	return;
}
