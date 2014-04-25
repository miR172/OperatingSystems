/*
**Inheritence from mmu: 
**Algorithm: NRU 
*/


#include "mmu.h"

#ifndef NRU_MMU_H
#define NRU_MMU_H


mmu* iniMMU_NRU();

int bits_NRU(pteAgent *myPage);

/* generic functions :*/
void updateMMU_NRU(mmu *m, int p);

int nextFrame_NRU(mmu *m);
/* generic functions */

int pagecmp_NRU(pteAgent *a, pteAgent *b);


#endif //NRU_MMU_H
