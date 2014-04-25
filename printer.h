/*
** macro for print 
*/

#ifndef PRINTER_H
#define PRINTER_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

extern int options[4];

#define PRINT_INSTHEAD(bit,pagenumber)	printf("==> inst: %1d %2d\n",bit, pagenumber)



#define PRINT_ZERO(instnumber, framenumber)		  \
  do {							  \
    if (options[0]==1){					  \
      printf("%d: ZERO      \t%d\n",instnumber, framenumber);	\
    }								\
  }while(0)


#define PRINT_INST(instnumber, instname, pagenumber, framenumber) \
  do {								  \
    if (options[0]==1){							\
      printf("%d: %-6s %d\t%d\n",instnumber,instname, pagenumber, framenumber);	\
    }									\
  } while(0)



#define PRINT_PAGE(id)				\
  do {							\
    if (isPresent(manager->pageT[id])){			\
      printf("%d:",id);					\
      if (manager->pageT[id].referenced){ putchar('R');}	\
      else {putchar('-');}					\
      if (manager->pageT[id].modified){ putchar('M');}		\
      else {putchar('-');}					\
      if (manager->pageT[id].pageout){ putchar('S');}		\
      else {putchar('-');}					\
    }								\
    else {							\
      if (manager->pageT[id].pageout){ putchar('#');}		\
      else { putchar('*');}					\
    }								\
    putchar(' ');						\
  } while (0)						



#define PRINT_FRAME(x)				\
    do {							\
      if (manager->frames[x] == -1){ printf("* ");}		\
      else { printf("%d ",manager->frames[x]); }		\
    } while (0)							


#endif
