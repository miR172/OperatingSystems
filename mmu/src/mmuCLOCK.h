/*
**Inheritence from mmu: 
**Algorithm: Clock based on virtual pages
*/


#include "mmu.h"

#ifndef CLOCK_MMU_H
#define CLOCK_MMU_H

mmu* iniMMU_Clock();

/* generic functions :*/
void updateMMU_Clock(mmu *m, int p);

int nextFrame_Clock(mmu *m);

/* generic functions */

#endif //CLOCK_MMU_H
