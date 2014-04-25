/*
**Inheritence from mmu: 
**Algorithm: Aging based on virtual pages
*/


#include "mmu.h"
#include "counter.h"
#include "mmuAging.h"
#include "utlist.h"
#include "pte.h"

extern int algo, numFrames, numPages;
extern mmu * manager;

mmu* iniMMU_Aging(){
  mmu* m = malloc(sizeof(mmu));
  m->pageT = iniPageTable();
  m->frames = iniFrames();
  m->pageA = NULL;
  inimmuStats(m);
  m->cT32 = iniCounter32();
  m->updateMMU = &updateMMU_Aging;
  m->nextFrame = &nextFrame_Aging;
  return m;
}


void updateMMU_Aging(mmu *m, int p){
  int i;
  for (i=0; i<numPages; i++){
    shiftAdd(&(m->cT32[i]), m->pageT[i].referenced);
    resetReference(&(m->pageT[i]));
  }
  return;
}

int nextFrame_Aging(mmu *m){
  m->pageA=NULL;
  int i;
  pteAgent *cur;
  for (i=0; i<numPages; i++){
    if (isPresent(m->pageT[i])){
      cur = iniPteAgent(i);
      DL_APPEND(m->pageA, cur);
    }
  }
  
  DL_SORT(m->pageA,pagecmp_Aging);
  
  cur = m->pageA;
  DL_DELETE(m->pageA,cur);
  int re = cur->myID;
  
  fireAgent(m->pageA);
  m->pageA=NULL;

  return re;
}

int pagecmp_Aging(pteAgent *a, pteAgent *b){//pteAgent.myID is page index
  c32 x = manager->cT32[a->myID];
  c32 y = manager->cT32[b->myID];
  if (x==y){return a->myID-b->myID; }
  if (x<y){return -1; }
  return 1;
}
