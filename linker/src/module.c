/*
** 	Implementation for Module Structure
**
** Each Module as it is in relocatable executable file (an input)
** 
** Declarations in module.h
** 
**	By Qinhua Yan @ NYU Feb 2014
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "utlist.h"
#include "variable.h"
#include "instruction.h"
#include "module.h"
#include "error.h"

extern int machineSize;	//define in main.c

void iniModule(Module *m){	//initialize
	m->index =0;
	m->start=0;
	m->codeCount=0;
	m->defCount=0;
	m->useCount=0;
	m->wList = NULL;
}

void setModule(Module * m){	
/* solve define list */
	Var *v;
	int i;
	Error *e;

	if (m->defCount!=0){ 
		v = m->defList; 	
	}
	for (i=0; i< m->defCount; i++){
		if (v->addr > m->codeCount){	//rule 5
			char msg[256];
			sprintf(msg, "Module %d: %s too big %d (max=%d) assume zero relative", m->index, v->name, v->addr, m->codeCount);
                        e = iniError(msg);
			LL_PREPEND(m->wList, e);
                        v->addr = 0;
		}
		v->addr += m->start;

		if (v->defined != m->index){
			v->defined = m->index;
		}
		v = v->next;
	}
}

void setInstruction(Module * m, Var * globalDef, int globalLen){
/* solve instruction list*/

	Instruction *in;	
	DL_FOREACH(m->instructionList,in){

		if (strcmp(in->type,"R")==0){ 
		/* Relative address */
			if (in->addr >= m->codeCount){ 
				in->errorID = iniError("Relative address exceeds module size; zero used");
				in->addr = 0;
			}			
			in->addr += m->start; /* relocated */
		}
		
		if (strcmp(in->type,"E")==0){
		/* External address */
			if (in->addr > m->useCount-1){ 
				in->errorID = iniError("External address exceeds length of uselist; treated as immediate");
				strcpy(in->type,"I");	
			} 
			else{
				int j = 0;
	                        Var *v = m->useList->prev; 
				Var *w = NULL;
				while (j!= in->addr){ 
/* iterate the define table-linked list to find corresponding var */
					j++;
					v = v->prev;
				}
				w = findVar(v, globalDef, globalLen); // find var by name, defined in variable.c
				if (w){ 
				//found
					//printf("peek: %s @ %d -> global %s @ %d\n",v->name, v->addr,w->name,w->addr);
					v->addr = w->addr;
					w->solved = 1;	/*FLAG USE*/
				}else{
				//not found
					char msg[256];
					sprintf(msg, "%s is not defined; zero used",v->name);
                                        in->errorID = iniError(msg);
                                        v->addr = 0;
				} 
				v->solved =1;	/*FLAG USE*/
				in->addr = v->addr;	/*relocate*/
			} 
		}
		
		if( strcmp(in->type,"A")==0 ){	
		/* Absolute address */
			if (in->addr > machineSize){
				in->errorID = iniError("Absolute address exceeds machine size; zero used");
				in->addr = 0;
			}
		} /* relocated */

		if ( strcmp(in->type,"I")==0 ){	
		/* Immediate address */
			if (in->op >9){
				in->errorID = iniError("Error: Illegal immediate value; treated as 9999");
				in->op = 9;				
				in->addr = 999;	
			}		
		}
	
	if (in->op > 9){
		Error *e = malloc(sizeof(*e));
				in->errorID = iniError("Error: Illegal opcode; treated as 9999");
				in->op = 9;				
				in->addr = 999;		
	}
	
	  in->relocated =1;
	}
	checkModule(m);
}

void checkModule(Module *m){
/* check use table of a module for non-use variables */
	m->wList = NULL;
	Var *v;
	DL_FOREACH(m->useList,v){
		if (v->solved!=1){
			Error *w;			
			char msg[256];
			sprintf(msg, "Module %d: %s appeared in the uselist but was not actually used", m->index, v->name);
			w = iniWarning(msg);
			LL_PREPEND(m->wList, w);
		}
	}	
}

