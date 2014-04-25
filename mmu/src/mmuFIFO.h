/*
**Inheritence from mmu: 
**Algorithm: FIFO
*/


#include "mmu.h"

#ifndef FIFO_MMU_H
#define FIFO_MMU_H

mmu* iniMMU_fifo();

/* generic functions :*/
void updateMMU_fifo(mmu *m, int p);

int nextFrame_fifo(mmu *m);

/* generic functions */

#endif //FIFO_MMU_H
