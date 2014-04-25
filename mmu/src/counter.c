/*
** Qinhua Yan @ NYU Mar 2014 
*/
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>

#include "counter.h"

extern c32 *cT32;
extern c64 *cT64;
extern int numPages;


void setAdd(c64 *c, c64 setthis){
  *c = setthis;
  return;
}

void shiftAdd(c32 *ca, int addthis){
  c32 leftm = 0x80000000;
  *ca >>=1;
  *ca += leftm*addthis;
  return;
}

void printBinary(c32 x){
  c32 i;
  for (i = 1 << 31; i>0; i=i/2){
    (i & x)?printf("1"):printf("0"); 
  }
  return;
}

