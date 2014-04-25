/*
**Inheritence from mmu: 
**Algorithm: FIFO
*/

#include <stdio.h>
#include <stdlib.h>
#include "mmu.h"
#include "mmuFIFO.h"
#include "utlist.h"

extern int algo, numFrames, numPages;


mmu* iniMMU_fifo(){
  mmu*m = malloc(sizeof(mmu));
  m->pageT = iniPageTable();
  m->frames = iniFrames();
  m->pageA = NULL;
  inimmuStats(m);
  iniCounters(m,0);
  m->updateMMU = &updateMMU_fifo;
  m->nextFrame = &nextFrame_fifo;
  return m;
}

void updateMMU_fifo(mmu *m, int p){
  if (m->pageA == NULL){
    int i;
    pteAgent *a;
    for (i=0; i<numFrames; i++){
      a = iniPteAgent(i); //frame index
      DL_APPEND(m->pageA, a);
    }
  }
  return;
}


int nextFrame_fifo(mmu *m){
  pteAgent *cur;
  int re;
  cur = m->pageA;
  re = cur->myID;
  DL_DELETE(m->pageA, cur);
  DL_APPEND(m->pageA, cur);
  return m->frames[re];
}


