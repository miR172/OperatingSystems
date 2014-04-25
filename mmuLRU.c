/*
**Inheritence from mmu: 
**Algorithm: LRU 
**Qinhua Yan @ NYU Apr 2014
*/

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "mmu.h"
#include "counter.h"
#include "mmuLRU.h"
#include "utlist.h"
#include "pte.h"
#include "printer.h"


extern int algo, numFrames, numPages;
extern mmu * manager;


mmu* iniMMU_lru(){
  mmu *m = malloc(sizeof(mmu));
  m->pageA = NULL;
  m->pageT = iniPageTable();
  m->frames = iniFrames();
  inimmuStats(m);
  m->cT64 = iniCounter64();
  m->updateMMU = &updateMMU_lru;
  m->nextFrame = &nextFrame_lru;
  return m;
}


void updateMMU_lru(mmu *m, int p){
  return;
}

int nextFrame_lru(mmu *m){
  m->pageA =NULL;
  int i;
  pteAgent *cur;
  for (i=0;i<numPages;i++){
    if (m->pageT[i].present==1){
      cur = iniPteAgent(i);
      DL_APPEND(m->pageA, cur);
    }
  }
  DL_SORT(m->pageA,pagecmp_lru);
  cur = m->pageA;
  DL_DELETE(m->pageA,cur);
  int re = cur->myID;
  fireAgent(m->pageA);
  m->pageA = NULL;
  return re;
}

int pagecmp_lru(pteAgent *a, pteAgent *b){ //pteAgent.myID is frame id
  c64 x = manager->cT64[a->myID];
  c64 y = manager->cT64[b->myID];
  if (x==y){
    if (manager->pageT[a->myID].frameid > manager->pageT[b->myID].frameid){
      return 1;
    }
    else {return -1;}
  }
  if (x<y) {return -1;}
  return 1;
}

