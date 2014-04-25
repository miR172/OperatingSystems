/*
**Inheritence from mmu: 
**Algorithm: NRU 
*/

#include <stdio.h>
#include <stdlib.h>
#include "mmu.h"
#include "mmuNRU.h"
#include "random.h"
#include "utlist.h"

extern int algo, numFrames, numPages;
extern FILE *rf;
extern mmu* manager;

mmu* iniMMU_NRU(){
  mmu *m = malloc(sizeof(mmu));
  m->pageT = iniPageTable();
  m->frames = iniFrames();
  m->pageA = NULL;
  inimmuStats(m);
  iniCounters(m,0);
  m->updateMMU = &updateMMU_NRU;
  m->nextFrame = &nextFrame_NRU;
  return m;
}


void updateMMU_NRU(mmu *m,int p){
  return;
}

int nextFrame_NRU(mmu *m){
  m->pageA=NULL;
  int i;
  pteAgent* cur;
  for (i=0; i<numPages; i++){
    if(isPresent(m->pageT[i])){
      cur = iniPteAgent(i); //indexed by page index
      DL_APPEND(m->pageA,cur);
    }
  }
  DL_SORT(m->pageA,pagecmp_NRU);
  /*  DL_FOREACH(m->pageA, cur){
    int i = cur->myID;
    printf("%d-%d%d ",i,m->pageT[i].referenced,m->pageT[i].modified);
  }
  printf("\n");
  */
  int classN = 0;
  int myN;
  pteAgent *headA = m->pageA; 
  int headN = bits_NRU(headA);
  DL_FOREACH(m->pageA, cur){
    myN = bits_NRU(cur);
    if (myN!=headN){
      break;
    }
    classN++;
  }
  int agent = getRDFrame(rf, classN);

  int j = 0;
  int pageIndex = 0;
  DL_FOREACH(m->pageA,cur){
    if (j==agent){
      pageIndex = cur->myID;
      break;
    }
    else { j++; }
  }
  fireAgent(m->pageA);
  m->pageA = NULL;
  int re = (m->replacementN)%10;
  if (re == 9){
    hardwareRefresh(m);
    //    printf("\n\n\t%ld refresh\n\n",m->replacementN);
  }
  m->replacementN += 1;
  return pageIndex;
}


int pagecmp_NRU(pteAgent *a, pteAgent *b){
  int b1 = bits_NRU(a);
  int b2 = bits_NRU(b);
  if (b1 == b2){ return a->myID - b->myID; }
      /* same class|order by page table Index*/ 
  return b1-b2;
}

int bits_NRU(pteAgent *a){
  int re =  2*(manager->pageT[a->myID].referenced) + (manager->pageT[a->myID].modified);
  return re;
}

