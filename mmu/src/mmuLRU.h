/*
**Inheritence from mmu: 
**Algorithm: LRU
*/


#include "mmu.h"

#ifndef LRU_MMU_H
#define LRU_MMU_H

mmu* iniMMU_lru();

/* generic functions :*/
void updateMMU_lru(mmu *m, int p);

int nextFrame_lru(mmu *m);
/* generic functions */

int pagecmp_lru(pteAgent *a, pteAgent *b);


#endif //LRU_MMU_H
