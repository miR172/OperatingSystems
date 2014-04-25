/*
**Inheritence from mmu: 
**Algorithm: Clock based on physical frames
*/


#include "mmu.h"

#ifndef FCLOCK_MMU_H
#define FCLOCK_MMU_H

mmu* iniMMU_fclock();

/* generic functions :*/

void updateMMU_fclock(mmu *m, int p);

int nextFrame_fclock(mmu *m);

/* generic functions */


#endif //FCLOCK_MMU_H
