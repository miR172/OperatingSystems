#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include "pte.h"
#include "algorithm.h"
#include "counter.h"
#include "instruction.h"
#include "printer.h"
#include "mmu.h"
#include "utlist.h"

extern int algo, numPages, numFrames;
extern inst* instList;
extern mmu* manager;
extern int options[4];

void clean();

pte* iniPageTable(){
  pte* p = malloc(sizeof(pte)*numPages);
  int i;
  for (i = 0; i<numPages; i++){
    iniPage(&(p[i]));
  }
  return p;
}

int* iniFrames(){
  int * f = malloc(sizeof(int)*numFrames);
  int i;
  for (i=0; i<numFrames; i++){
    f[i]=-1;
  }
  return f;
}

c64* iniCounter64(){
  c64 *c = malloc(sizeof(c64)*numPages);
  int i;
  for (i=0; i<numPages; i++){
      c[i] = 0;
  }
  return c;
}

c32* iniCounter32(){
  c32 *c = malloc(sizeof(c32)*numPages);
  int i;
  for (i=0; i<numPages; i++){
      c[i] = 0;
  }
  return c;
}

void iniCounters(mmu*m, int a){
  m->cT32 = NULL;
  m->cT64 = NULL;
  return;
}

void inimmuStats(mmu *m){
  m->replacementN=0;
  m->mmucounter =0;
  m->insts_N =0;
  m->unmaps_N=0;
  m->maps_N=0;
  m->ins_N=0;
  m->outs_N=0;
  m->zeros_N=0;
  m->totalcost = 0;
}


void updateMMU(mmu *m, int p){
  if (m->updateMMU!=NULL){
    m->updateMMU(m, p);
  } else {
    return;
  }
}


int nextFrame(mmu *m){
  if (m->nextFrame!=NULL){
    return m->nextFrame(m);
  } else {
    printf("MMU Unitialized.\nExit.\n");
    clean(); //will force quit
    return 0;
  }
}

void hardwareRefresh(mmu *m){
  int i ;
  for (i=0; i<numPages; i++){
    resetReference(&(m->pageT[i]));
  }
  return;
}


int unmap(mmu *m, int index, int instnnd){
  int re =  m->pageT[index].frameid;
  char instname[6];
  strcpy(instname, "UNMAP");
  PRINT_INST(instnnd, instname, index, re);

  resetPresent(&(m->pageT[index]));
  resetReference(&(m->pageT[index]));

  if (m->pageT[index].modified == 1){
    pageOut(m, index, instnnd, re); 
  }
  resetModified(&(m->pageT[index]));

  m->frames[re] = -1;
  m->frameN -= 1;
  resetFrameid(&(m->pageT[index]));

  m->unmaps_N += 1;

  return re;
}

void pageOut(mmu *m, int index, int instnnd, int phy){

  char instname[6];
  strcpy(instname, "OUT");
  PRINT_INST(instnnd, instname, index, phy);

  resetModified(&(m->pageT[index]));
  pagedout(&(m->pageT[index]));
  
  m->outs_N += 1;

  return;
}

void pageIn(mmu *m, int index, int instruction, int instnnd, int phy){
  
  if (m->pageT[index].pageout==1) {

    char instname[6];
    strcpy(instname,"IN");
    PRINT_INST(instnnd, instname, index, phy);
    zeroIn(&(m->pageT[index]));
    m->ins_N += 1;

  } else {
    PRINT_ZERO(instnnd, phy); 
    iniPage(&(m->pageT[index]));
    m->zeros_N +=1;
  }

  if (instruction==1){
    modified(&(m->pageT[index])); 
  }

  return;
}

void map(mmu *m, int index, int phy, int instnnd){
  PRINT_INST(instnnd, "MAP", index, phy);
  present(&(m->pageT[index]));
  referenced(&(m->pageT[index]));
  pagedin(&(m->pageT[index]),phy); 
  m->frames[phy] = index;
  m->frameN += 1;  
  m->maps_N += 1;
  return;
}


void simulate(mmu *m){
  inst *elt, *tmp;
    
  while (instList != NULL){
    tmp = instList;
    elt = popInst(tmp);
    if (elt == NULL){ continue; } //invalid instruction

    m->insts_N +=1;
    
    int rw = elt->rw;
    int page = elt->page;
    int instN = elt->instN;
    free(elt); elt =NULL; tmp = NULL;

    if (algo==3){
      m->mmucounter +=1;
      m->cT64[page] =  m->mmucounter; 
      //printf("==> %llu %lu\n", m->cT64[page],m->mmucounter);
    }

    if (isPresent(m->pageT[page])){
      if (rw ==1){ 
	modified(&(m->pageT[page])); 
      }
      referenced(&(m->pageT[page]));
      continue; 
    } //page present 
   

    int victim, nextFID;
    if (m->frameN < numFrames){
      victim = page;
      nextFID = m->frameN;
    }
    else {
      updateMMU(m, page);
      victim = nextFrame(m);
      nextFID = unmap(m,victim,instN); 
    }

    pageIn(m, page, rw, instN, nextFID); 
    
    map(m, page, nextFID, instN); /* MAP */
    
  } //while instList NOT NULL
  
  return;
}


void calculator(mmu *m){
  m->totalcost = 0;
  m->totalcost += 400*m->unmaps_N;
  m->totalcost += 400*m->maps_N;
  m->totalcost += 3000*m->ins_N;
  m->totalcost += 3000*m->outs_N;
  m->totalcost += 150*m->zeros_N;
  m->totalcost += m->insts_N;
  return;
}



