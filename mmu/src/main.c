
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "utlist.h"
#include "pte.h"
#include "counter.h"
#include "mmu.h"
#include "mmuNRU.h"
#include "mmuRandom.h"
#include "mmuFIFO.h"
#include "mmuSC.h"
#include "mmufclock.h"
#include "mmuCLOCK.h"
#include "mmuLRU.h"
#include "mmuAging.h"
#include "mmuage.h"
#include "algorithm.h"
#include "instruction.h"
#include "printer.h"
#include "random.h"

FILE *fp, *instf, *rf;
char buffer[256];
int options[4];
int algo, numFrames, numPages;
inst * instList;
mmu* manager;


void clean();
void iniGlobal();
void checkPar();
void printerF();

int main(int argc, char *argv[]){
  
  iniGlobal();
  
  int i = 1;
  char algotmp;
  char *numFramesChar;

  for (;i<argc;i++){
    strcpy(buffer,argv[i]);
    fp = fopen(argv[i],"r");
    
    if (fp==NULL){
      if (argv[i][0]!='-'){
	break; 
      }
      int j;
      switch (argv[i][1]){
        case 'f': 
	  numFramesChar = &(argv[i][2]);
	  numFrames = atol(numFramesChar);
	  break;
        case 'a':
	  algotmp = argv[i][2];	  
	  break;
        case 'o':
	  j = 2;
	  while(argv[i][j]){
	    switch (argv[i][j]){
	    case 'O': options[0]=1;break;
	    case 'P': options[1]=1;break;
	    case 'F': options[2]=1;break;
	    case 'S': options[3]=1;break;
	    }
	    j++;
	  }
	  break;
      }
    }
    else{
      if (instf ==NULL) { instf = fp;}
      else { rf = fp; }
    }
  }
  /* end op parsing. begin load data */
  
  algo = iniAlgorithm(algotmp);
  checkPar();
  readInstFile(instf);
  fclose(instf);
  checkInstList();
  iniRDFile(rf);
  /*all checks pass. safe to start simulate */

  //printf("numFrames %d numPages %d options[OPFS] %d%d%d%d ", numFrames, numPages, options[0], options[1], options[2],options[3]);
  //printAlgorithm(algo);
  manager->algo = algo;
  simulate(manager);
  calculator(manager);
  printerF();
  if (rf != NULL){ fclose(rf);}
  clean();
  return 0;
}


void iniGlobal(){
  algo = 0;
  instList = NULL;
  fp = NULL;
  instf= NULL;
  rf = NULL;
  manager = NULL;
  numFrames = 0;
  numPages = 64;
  int i;
  for (i=0;i<4;i++){ options[i] = 0;}
}

void checkPar(){
  if (algo==0) {
    printf("Wrong Algorithm.\nExit\n");
    clean();
  }
  
  if (numFrames <= 0){ 
    printf("Number of Frames <= 0.\nExit\n");
    clean();
  }
  if (instf==NULL){
    printf("Please indicate instruction file.\nExit.\n");
    clean();
  }
  if ((algo == 1) || (algo == 2)){
    if (rf ==NULL){
      printf("Please indicate random file.\nExit.\n");
      clean();
    }}
}


void clean(){
  if (algo > 5){fireAgent(manager->pageA);} 
  if (manager->pageT!=NULL){free(manager->pageT);}
  if (manager->frames!=NULL){free(manager->frames);}
  if (algo == 3) {
    if (manager->cT64!=NULL){free(manager->cT64);}}
  if ((algo == 4) || (algo==5)) {
    if (manager->cT32!=NULL){free(manager->cT32);}} 
  free(manager);
  instList = NULL;
  manager = NULL;
  exit(0);
}


void printerF(){
  int i;
  if (options[1]==1){
    for (i=0; i<numPages; i++){ PRINT_PAGE(i);}
    printf("\n");
  }
  if (options[2]==1){
    for (i=0; i<numFrames; i++){ PRINT_FRAME(i);}
    printf("\n");
  }
  if (options[3]==1){
    printf("SUM %ld U=%ld M=%ld I=%ld O=%ld Z=%ld ===> %llu\n", manager->insts_N, manager->unmaps_N, manager->maps_N, manager->ins_N, manager->outs_N, manager->zeros_N, manager->totalcost);
  }
  return;
}
