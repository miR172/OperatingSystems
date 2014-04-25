/*
**Inheritence from mmu: 
**Algorithm: Aging based on physical frames
*/

#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include "mmu.h"
#include "counter.h"
#include "mmuage.h"
#include "mmuAging.h"
#include "utlist.h"
#include "pte.h"

extern int algo, numFrames, numPages;
extern mmu * manager;

mmu* iniMMU_age(){
  mmu* m = malloc(sizeof(mmu));
  m->pageT = iniPageTable();
  m->frames = iniFrames();
  m->pageA = NULL;
  inimmuStats(m);
  m->cT32 = iniCounter32();
  m->updateMMU = &updateMMU_age;
  m->nextFrame = &nextFrame_age;
  return m;
}


void updateMMU_age(mmu *m, int p){
  updateMMU_Aging(m, p);
  return;
}

int nextFrame_age(mmu *m){
  m->pageA=NULL;
  int i, re;
  pteAgent * cur;
  for (i=0; i<numPages; i++){
    if (isPresent(m->pageT[i])){
      cur = iniPteAgent(i);
      DL_APPEND(m->pageA, cur);
    }
  }
    
  DL_SORT(m->pageA,pagecmp_age);
  cur  = m->pageA;
  DL_DELETE(m->pageA,cur);
  re = cur->myID;
  
  fireAgent(m->pageA);
  m->pageA=NULL;
  
  return re;
}

int pagecmp_age(pteAgent *a, pteAgent *b){//pteAgent.myID is frame index
  c32 x = manager->cT32[a->myID];
  c32 y = manager->cT32[b->myID];
  if (x==y){return manager->pageT[a->myID].frameid - manager->pageT[b->myID].frameid; }
  if (x<y){return -1; }
  return 1;
}
