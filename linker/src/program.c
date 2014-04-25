/*
** 	Implementation of Program Structure
** 
** One Program Structure for each input ( a relocatable exe file ) 
** Declarations in program.c
**
**	 By Qinhua Yan @ NYU Feb 2014
*/


#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "utlist.h"
#include "module.h"
#include "variable.h"
#include "instruction.h"
#include "program.h"
#include "error.h"

void iniProgram(Program * p){
/* initialize */
	p->varCount = 0;
	p->instructionCount = 0;
	p->moduleCount = 0;
	p->moduleList = NULL;
	p->globalList=NULL;
	p->instructions=NULL;
}

Program parse(FILE *fp){
	
	Program *program = malloc(sizeof(Program));
	iniProgram(program);	/* initialized */
	
	int defineN, useN, instructionN, i, instructionCode, addr; 
	int position = 0; //counter
	int token = 1; //counter
	char name[17], type[2];

	while (fscanf(fp,"%d",&defineN) == 1){
		//new module, define list size got
		token++;
		program->moduleCount++;
		
		Module *m = malloc(sizeof(Module));
		iniModule(m);
		m->defList = NULL;
		m->useList = NULL;
		m->index = program->moduleCount;
		m->defCount = defineN;
		
		for (i=0; i<defineN; i++){
		/* scan until get desired number of codes */	
			Var *v = malloc(sizeof(Var));
			iniVar(v);
			v->defined = m->index;
			
			if (fscanf(fp,"%s",name)==1 ){ token++; }
			else{ syntaxError(1,token); }

			if ( fscanf(fp,"%d",&addr)==1 ){ token++; }
			else{ syntaxError(2,token);  }
/* if the following strings don't obey the pattern 
** must be syntax error, thus raise it. */
			strcpy(v->name, name);
			v->addr = addr;
			DL_PREPEND(m->defList, v);
		}	
		/* define list get */


		if ( fscanf(fp,"%d",&useN)==1 ){ token++; }
		else{ syntaxError(4, token);  }
		/* uselist size got */

		m->useCount = useN;
		for (i=0; i<useN; i++){
		/* scan until get desired number of codes */
			Var *v = malloc(sizeof(Var));
			iniVar(v);
			if ( fscanf(fp,"%s",name)==1 ){ token++;}
			else{ syntaxError(1,token);  }
			strcpy(v->name, name);
			DL_PREPEND(m->useList, v);
		}
		/* use list get */
		
		if ( fscanf(fp,"%d",&instructionN) ==1 ){token++;}
		else{ syntaxError(2,token);  }
		/* instruction size get */

		m->codeCount = instructionN;
		for (i=0; i<instructionN; i++){
		/* scan until get desired number of codes */
			Instruction *in = malloc(sizeof(Instruction));
			iniInstruction(in);
			if (fscanf(fp,"%1s", type) ==1) {token++;}
			else{ syntaxError(1,token);  }
			if (fscanf(fp,"%d", &instructionCode)==1) {token++;}
			else{ syntaxError(0,token);  }
			strcpy(in->type, type);
			in->addr = instructionCode%1000;
			in->op = instructionCode/1000;
			DL_PREPEND(m->instructionList, in);
		}
		/* instruction list get */

                m->start = position;     
		DL_PREPEND(program->moduleList, m); 
		/* add module to program's list */
		position += m->codeCount;
	}
	/* while loop go on to the next module's start
	   until EOF */
	return *program; 
	/* everything stores via pointer in program 
	** it is really a collection of all the information we need 
	** we can let the stack dies out 
	** and stick on to the pointer of the program*/
}

void solve(Program *p){
/* pass 2 */
	int c; //counter
	
	Module *m = p->moduleList;
	for (c=0; c<p->moduleCount;c++){
//	DL_FOREACH(p->moduleList,m)
		setModule(m);
		/* define in module.c relocates global variables */
		Var *v = m->defList;
		Var *w;
		int cc; 
		for (cc=0; cc< m->defCount; cc++){ //for each var on deflist
			w = NULL;
			w = findVar(v, p->globalList, p->varCount); 
			if (!w){ //not found
				Var *new = malloc(sizeof(Var));
				iniVar(new);
				strcpy(new->name,v->name);
				new->addr = v->addr;
				new->defined = v->defined;
				new->errorID = v->errorID; 
				DL_PREPEND(p->globalList, new); //copy and add to global table
                                p->varCount++;
			}else{ //found
				if (w->defined > v->defined){ //v is ealier
					w->defined = v->defined;
					w->addr = v->addr;
				}
                        	w->errorID = iniError("This variable is multiple times defined; first value used");
			}
			v = v->next;
		}
		m = m->next;
	}	
	/* global variables added to program's table */

	Module *mm = p->moduleList->prev; 
	c = 0;
	while (c < p->moduleCount){
		printWarning(mm->wList);
		mm->wList = NULL;
		mm = mm->prev;
		c++;
	} 
	/* print warnings for rule 5 */

	c = 0;
	DL_FOREACH(p->moduleList,m){
		setInstruction(m, p->globalList,p->varCount);
		/* defined in module.c, relocates instructions */
		Instruction *in, *tmp;
		DL_FOREACH_SAFE(m->instructionList,in,tmp){
			DL_PREPEND(p->instructions,in);
			DL_DELETE(m->instructionList,in);
			c++;
		} 
	}
	/* save instructions to program's instruction table */
	p->instructionCount = c;
} /* solve variables in instruction list of type R and E*/


void checkProgram(Program *p){
/* rule 4 */
	Var *v = p->globalList;	//head
	int c;
	for (c=0; c < p->varCount; c++){
		if (v->solved != 1){
		printf("Warning: Module %d: %s was defined but never used\n",v->defined,v->name);
		}
		v = v->next;
	}
}

void print(Program *p){	 
/*print and final error checks*/
	printf("\nSymbol Table\n");
	Var *v = p->globalList;	//head
	int i=0;
	while (i < p->varCount){
		printVar(v);	
		i++;
		v = v->next;
	} 
	/* print global variable table */

	printf("\nMemory Map\n");	
	Instruction *in = p->instructions;
	i=0;
	while (i< p->instructionCount){
		printf("%03d: ",i);
		printCode(in, 0);
		in = in->next;
		i++;	
	} 
	/* print instruction table */
	
	printf("\n");
	checkProgram(p);
	
	Module *m = p->moduleList;
	i = 0;
	while (i < p->moduleCount){
		printWarning(m->wList);
		m = m->next;
		i++;
	}
	/* print warnings */ 
}	
