/*
**Inheritence from mmu: 
**Algorithm: SecondChance
*/


#include "mmu.h"

#ifndef SECONDCHANCE_MMU_H
#define SECONDCHANCE_MMU_H

mmu* iniMMU_sc();

/* generic functions :*/
void updateMMU_sc(mmu *m, int p);

int nextFrame_sc(mmu *m);

/* generic functions */

#endif //SECONDCHANCE_MMU_H
