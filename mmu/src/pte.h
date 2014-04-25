/*                                                                           
** Header for Page-Table-Entry pte Structure                       
** Implementation in pte.c                                               
** By Qinhua Yan @ NYU Mar 2014                                        
*/

#ifndef PTE_H
#define PTE_H


#include <stdio.h>
#include <stdlib.h>
#include <string.h>


typedef struct __PAGE_TABLE_ENTRY {
  unsigned int present: 1;
  unsigned int referenced: 1;  
  unsigned int modified: 1;
  unsigned int pageout: 1;
  int frameid: 28; 
} pte;

/* setters | resetters: flagged | change back to 0 */
/* pass in pointer for modify*/

typedef struct __PAGE_TABLE_ENTRY_AGENT{
  int myID;
  struct __PAGE_TABLE_ENTRY_AGENT *prev, *next;
}pteAgent;


void iniPage(pte *);

pteAgent * iniPteAgent(int);

void fireAgent(pteAgent *);

void resetPresent(pte *);

void resetReference(pte *);

void resetModified(pte *);

void resetPageout(pte *);

void resetFrameid(pte *);

void zeroIn(pte *);

void present(pte *);

void referenced(pte *);

void modified(pte *);

void pagedout(pte *);

void pagedin(pte *,int);


/* printers: never modifies, pass in object */
int isPresent(pte);

void printPage(pte);


#endif
