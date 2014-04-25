/* Implementation for instructions
** Declaration in instruction.h
** Qinhua Yan @ NYU Mar 2014
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "utlist.h"
#include "instruction.h"
#include "printer.h"

extern inst *instList;
void clean();


void readInstFile(FILE * f){
  int one, two;
  int n = 0;
  inst *cur;  
  char cmtl[9999];
  while (!feof(f)){
    if ((fscanf(f,"%d",&one))==1){
      if ((fscanf(f,"%d",&two))==1){
	cur = iniIncode(one, two, n);
	appendTo(cur);
	n++;
      }
    } else {
      fgets(cmtl, 9999, f); 
      continue;
    }
  }
}

inst * iniIncode(int a, int b, int n){
  inst *i;
  i = malloc(sizeof(inst));
  i->rw = a;
  i->page = b;
  i->instN = n;
  i->prev = NULL;
  i->next = NULL;
  return i;
}

void appendTo(inst * thisInst){
  DL_APPEND(instList, thisInst);
}

inst* popInst(inst * thisInst){
  if (instList ==NULL){ return NULL; }
  DL_DELETE(instList, thisInst);
  if ((thisInst->rw <0 ) || ( thisInst->rw > 1)) {
    printf("Illegal Instruction Code: %d @ %d\nUse Next Instruction.\n",thisInst->rw, thisInst->instN);
    free(thisInst);
    return NULL;
  }
  if ((thisInst->page < 0) || (thisInst->page > 63)){
    printf("Illegal Page Index: %d \nUse Next Instruction.\n",thisInst->page);
    free(thisInst);
    return NULL;
  }
  if (options[0]==1){ PRINT_INSTHEAD(thisInst->rw, thisInst->page);}

  return thisInst;
}


void checkInstList(){
  if (instList==NULL){
    printf("Empty Instruction File.\nExit.\n");
    clean();
  }
  return;
}

void printInst(inst * i){
  printf("inst: %d %d %d\n",i->instN, i->rw, i->page);
}
