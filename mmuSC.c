/*
**Inheritence from mmu: 
**Algorithm: SecondChance
*/

#include <stdio.h>
#include <stdlib.h>
#include "mmu.h"
#include "mmuFIFO.h"
#include "mmuSC.h"
#include "utlist.h"

extern int algo, numFrames, numPages;
extern mmu* manager;

mmu* iniMMU_sc(){
  mmu *m = malloc(sizeof(mmu));
  m->pageT = iniPageTable();
  m->frames = iniFrames();
  m->pageA = NULL;
  inimmuStats(m);
  iniCounters(m,0);
  m->updateMMU = &updateMMU_sc;
  m->nextFrame = &nextFrame_sc;
  return m;
}

void updateMMU_sc(mmu *m,int p){
  if (m->pageA == NULL){
    int i;
    pteAgent *a;
    for (i=0; i<numFrames; i++){
      a = iniPteAgent(i); //frame index
      DL_APPEND(m->pageA, a);
    }
  }
  pteAgent *elt;
  elt = m->pageA;
  while (m->pageT[m->frames[elt->myID]].referenced != 0){
    DL_DELETE(m->pageA,elt);
    resetReference(&(m->pageT[m->frames[elt->myID]]));
    DL_APPEND(m->pageA,elt);
    elt = m->pageA;
  }
  return;
}


int nextFrame_sc(mmu *m){
  int re = nextFrame_fifo(m);
  return re;
}

