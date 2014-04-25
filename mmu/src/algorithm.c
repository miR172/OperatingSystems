#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include "algorithm.h"
#include "mmu.h"
#include "mmuNRU.h"
#include "mmuRandom.h"
#include "mmuFIFO.h"
#include "mmufclock.h"
#include "mmuSC.h"
#include "mmuCLOCK.h"
#include "mmuLRU.h"
#include "mmuAging.h"
#include "mmuage.h"

extern mmu* manager;

const char* algorithms[]= {" ", "random", "NRU", "LRU", 
			   "aging_PhysicalFrames", "aging_VirtualPages", "FIFO", 
			   "SecondChance","clock_PhysicalFrames","clock_VirtualPages"};

int iniAlgorithm(char s){
  switch (s){
  case 'N': 
    manager = iniMMU_NRU();
    return 2; break;
  case 'l': 
    manager = iniMMU_lru();
    return 3; break;
  case 'r': 
    manager = iniMMU_Random();
    return 1; break;
  case 'f': 
    manager = iniMMU_fifo();
    return 6; break;
  case 's': 
    manager = iniMMU_sc();
    return 7; break;
  case 'c': 
    manager = iniMMU_fclock();
    return 8; break;
  case 'a': 
    manager = iniMMU_age();
    return 4; break;
  case 'C': 
    manager = iniMMU_Clock();
    return 9; break;
  case 'A': 
    manager = iniMMU_Aging();
    return 5; break;
  default: return 0;
      }
}


void printAlgorithm(int a){
  printf("%s\n", algorithms[a]);
}





