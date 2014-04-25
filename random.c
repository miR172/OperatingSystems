/*
** Qinhua Yan @ NYU Mar 2014
*/

#include <stdlib.h>
#include <stdio.h>
#include "random.h"

extern FILE *rf;
extern int algo;

void clean();


void iniRDFile(FILE * rf){
  if ((algo == 1) || (algo == 2)){
    getRD(rf);
  }
  return;
}

int getRD(FILE * rf){
  int rd; 
  int tmp;
  if (fscanf(rf, "%d", &rd)==1){
    return rd;
  }
  rewind(rf); //jump to the head
  fscanf(rf,"%d", &tmp); //skip the first number                   
  if ( fscanf(rf, "%d", &rd) != 1 ){
    printf("Destructive Random File. Please use another random number file.\nExit.\n");
    clean(); //will force quit
  }
  return rd;
}

int getRDFrame(FILE *rf, int base){
  int rd = getRD(rf);
  //  printf("random peek: rd %d base %d give %d\n",rd, base, rd%base);//
  return rd%base;
}
