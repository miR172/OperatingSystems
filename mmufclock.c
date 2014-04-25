/*
**Inheritence from mmu: 
**Algorithm: Clock based on physical frames 
*/

#include <stdlib.h>
#include <stdio.h>
#include "mmu.h"
#include "mmuFIFO.h"
#include "mmuSC.h"
#include "mmuCLOCK.h"
#include "mmufclock.h"
#include "utlist.h"

extern int algo, numFrames, numPages;
extern mmu* manager;

mmu* iniMMU_fclock(){
  mmu *m = malloc(sizeof(mmu));
  m->pageT = iniPageTable();
  m->frames = iniFrames();
  m->pageA = NULL;
  inimmuStats(m);
  iniCounters(m,0);
  m->updateMMU = &updateMMU_fclock;
  m->nextFrame = &nextFrame_fclock;
  return m;
}

void updateMMU_fclock(mmu *m, int p){
  updateMMU_sc(m, p);
  return;
}


int nextFrame_fclock(mmu *m){
  int re = nextFrame_fifo(m);
  return re;
}



