/* mmu Generic Header File
** This file declares mmu structure with generic functions for replacement algorithms.
** Qinhua Yan @ NYU Mar 2014
*/

#ifndef ANIMAL_H
#define ANIMAL_H

#include "pte.h"
#include "algorithm.h"
#include "instruction.h"
#include "counter.h"
#include "stdint.h"

struct __STRUCT_mmu;
typedef void (*MMU_FUNC_UPDATEMMU) (struct __STRUCT_mmu *m, int p);
typedef int (*MMU_FUNC_NEXTFRAME) (struct __STRUCT_mmu *m);

typedef struct __STRUCT_mmu{

  pte *pageT;
  pteAgent *pageA;
  int *frames;
  int frameN;

  int algo;

  MMU_FUNC_UPDATEMMU updateMMU;
  MMU_FUNC_NEXTFRAME nextFrame;

  signed long replacementN, insts_N, unmaps_N, maps_N, ins_N, outs_N, zeros_N;
  unsigned long long totalcost;
  c32 mmucounter;
  c64 *cT64;
  c32 *cT32;
} mmu;

pte* iniPageTable();

int* iniFrames();

c64* iniCounter64();

c32* iniCounter32();

void iniCounters(mmu*m, int a);

void inimmuStats(mmu *m);

/* generic functions :*/
void updateMMU(mmu *m, int p);

int nextFrame(mmu *m);

/* generic functions */


void hardwareRefresh(mmu *m);

int unmap(mmu *m, int index, int instnnd);

void pageOut(mmu *m, int index, int instnnd, int phy);

void pageIn(mmu *m, int index, int instruction, int instnnd, int phy);

void map(mmu *m, int index, int phy, int instnnd);

void simulate(mmu *m);

void calculator(mmu *m);


#endif
