/*
**Inheritence from mmu: 
**Algorithm: Aging based on virtual pages
*/


#include "mmu.h"

#ifndef AGING_MMU_H
#define AGING_MMU_H

mmu* iniMMU_Aging();

/* generic functions :*/
void updateMMU_Aging(mmu *m, int p);

int nextFrame_Aging(mmu *m);

int pagecmp_Aging(pteAgent *a, pteAgent *b);
/* generic functions */

#endif //Aging_MMU_H
