/*
**Inheritence from mmu: 
**Algorithm: Random
*/


#include "mmu.h"
#include "mmuRandom.h"
#include "random.h"

extern int algo,numFrames, numPages;
extern FILE * rf;

mmu* iniMMU_Random(){
  mmu *m = malloc(sizeof(mmu));
  m->pageT = iniPageTable();
  m->frames = iniFrames();
  m->pageA = NULL;
  inimmuStats(m);
  iniCounters(m,0);
  m->algo = algo;
  m->updateMMU = &updateMMU_Random;
  m->nextFrame = &nextFrame_Random;
  return m;
}


void updateMMU_Random(mmu *m, int p){ return; } 

int nextFrame_Random(mmu *m){
  int phy = getRDFrame(rf, numFrames);
  return m->frames[phy];
}



