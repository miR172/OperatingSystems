Qinhua Yan 2014 Apr
Operating Systems Lab3 MMU

All codes are in C. 
To compile, switch to src/

	$ gcc *.c -o mmu

(compile all *.c codes there) will generate executable file mmu

./mmu accepts parameters as described in the assignment
Order of the parameters doesn't matter, except that 
in algorithms that take input and randomfile, 
the input file must appear first.

>---------------------------------------------------------<
 Inheritence:

parent structures: mmu, decalred in "mmu.h"

mmu*.h defined child structures. 
The child mmu class share the same functions 
simulate(), map(), unmap()... defined in "mmu.h"

Each of them has its own version of

	void updateMMU(mmu *,int); //updateMMU's tables
	int nextFrame(mmu *); //get a page to evict

saved as pointers in their struct

Particular mmu subclass is envoked by 
	void iniMMU_*(mmu *);

which is also declared in its own class header file.


Other header files: 
pte.h, algorithm.h, instruction.h, random.h, counter.h declare utilities. 
utlist.h and printer.h are macros. 


