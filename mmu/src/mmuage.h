/*
**Inheritence from mmu: 
**Algorithm: Aging based on physical frames
*/


#include "mmu.h"

#ifndef F_AGING_MMU_H
#define F_AGING_MMU_H

mmu* iniMMU_age();

/* generic functions :*/
void updateMMU_age(mmu *m, int p);

int nextFrame_age(mmu *m);

int pagecmp_age(pteAgent *a, pteAgent *b);
/* generic functions */

#endif //F_AGING_MMU_H
