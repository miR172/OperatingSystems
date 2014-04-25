/*
**Inheritence from mmu: 
**Algorithm: Clock based on virtual pages
*/

#include <stdlib.h>
#include <stdio.h>
#include "mmu.h"
#include "mmuFIFO.h"
#include "mmuSC.h"
#include "mmuCLOCK.h"
#include "utlist.h"

extern int algo, numFrames, numPages;
extern mmu* manager;

mmu* iniMMU_Clock(){
  mmu* m = malloc(sizeof(mmu));
  m->pageT = iniPageTable();
  m->frames = iniFrames();
  m->pageA = NULL;
  inimmuStats(m);
  iniCounters(m,0);
  m->updateMMU = &updateMMU_Clock;
  m->nextFrame = &nextFrame_Clock;

  return m;
}

void updateMMU_Clock(mmu *m, int p){
    if (m->pageA == NULL){
      int i;
      pteAgent *a;
      for (i=0; i<numPages; i++){
	a = iniPteAgent(i); //agents indexed by page index
	DL_APPEND(m->pageA, a);
      }
    }
    pteAgent *elt, *tmp;
    DL_FOREACH_SAFE(m->pageA, elt, tmp){
      if ((isPresent(m->pageT[elt->myID])) && 
	  (m->pageT[elt->myID].referenced == 0)){ break; }
      DL_DELETE(m->pageA,elt);
      if (isPresent(m->pageT[elt->myID])){
	resetReference(&(m->pageT[elt->myID]));
      }
      DL_APPEND(m->pageA,elt);
    }
    /*    DL_FOREACH(m->pageA, elt){
      printf("%d-", elt->myID);
      printf("%d ", m->pageT[m->frames[elt->myID]].referenced);
    }
    printf("\n\n");*/
    return;
}


int nextFrame_Clock(mmu *m){
  pteAgent *cur;
  int re;
  cur = m->pageA;
  re = cur->myID;
  DL_DELETE(m->pageA, cur);
  DL_APPEND(m->pageA, cur);
  return re;
}


