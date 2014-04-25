/*
**Inheritence from mmu: 
**Algorithm: Random
*/


#include "mmu.h"

#ifndef RANDOM_MMU_H
#define RANDOM_MMU_H


mmu* iniMMU_Random();

/* generic functions :*/
void updateMMU_Random(mmu *m, int p);

int nextFrame_Random(mmu *m);

/* generic functions */

#endif //RANNDOM_MMU_H
