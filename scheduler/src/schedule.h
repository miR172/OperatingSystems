/*
** Header for Schedule  Structure
** Implementation in schedule.c                                 
** By Qinhua Yan @ NYU Mar 2014                                 
*/

#ifndef SCHEDULE
#define SCHEDULE

#include <stdio.h>
#include <stdlib.h>
#include "ioManager.h"
#include "process.h"
#include "utlist.h"

typedef struct __STRUCT_schedule{
  process * fl; //list of finished process
  int pN; //number of process
  int algo; //[0, F, L, S, R]
  int quan; //quantum
} schedule;

void iniSchedule(FILE *, char, int);

int fSort(process *, process *);
int lSort(process *, process *);
int sSort(process *, process *);
int rSort(process *, process *);

void cpuWait();
void returnCandidate();
process *getNext();
void simulator();

void schedulePrinter();


#endif
