/*                                                                           
** Implementation for Page-Table-Entry pte Structure                       
** Declaration in pte.h                                               
** By Qinhua Yan @ NYU Mar 2014                                        
*/


#include "pte.h"
#include <stdio.h>
#include <stdlib.h>
#include "utlist.h"
#include "mmu.h"

extern int numPages;

void iniPage(pte *p){
  zeroIn(p);
  resetPageout(p);
}

pteAgent* iniPteAgent(int x){
  pteAgent * pa= malloc(sizeof(pteAgent));
  pa->myID = x;
  return pa;
}

void fireAgent(pteAgent *a){
  if (a==NULL){return;}
  pteAgent *elt, *tmp;
  DL_FOREACH_SAFE(a,elt,tmp){
    DL_DELETE(a, elt);
    free(elt);
    elt=NULL;
  }
  a = NULL;
  return;
}

void resetPresent(pte *p){
  p->present = 0;
}

void resetReference(pte *p){
  p->referenced = 0;
}

void resetModified(pte *p){
  p->modified = 0;
}

void resetPageout(pte *p){
  p->pageout = 0;
}

void resetFrameid(pte *p){
  p->frameid = 0;
}

void zeroIn(pte *p){
  resetPresent(p);
  resetReference(p);
  resetModified(p);
  resetFrameid(p);
}


void present(pte *p){
  p->present = 1;
}

void referenced(pte *p){
  p->referenced = 1;
}

void modified(pte *p){
  p->modified = 1;
}

void pagedout(pte *p){
  p->pageout = 1;
}

void pagedin(pte *p, int phyID){
  p->frameid = phyID;
}

int isPresent(pte p){
  return p.present;
}

void printPage(pte p){
  printf("%d-%d-%d-%d-%d\n",p.present,p.referenced, p.modified, p.pageout,p.frameid);
}
