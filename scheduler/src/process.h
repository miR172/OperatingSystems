/*                                                                           
** Header for Process  Structure                       
** Implementation in process.c                                               
** By Qinhua Yan @ NYU Mar 2014                                        
*/

#ifndef PROCESS
#define PROCESS

#include "utlist.h"
#include "ioManager.h"
#include <stdio.h>

typedef struct __STRUCT_process{
  int pid;

  int status, scheduled; 
  /* [-1=Finished, 0=Ready/Can't be Schedule, 1 or larger=Schedulable] | 
     [0, 1-Scheduled] */

  int at,tc,cb,io; 
	/*arrival time | total cpu time | cpu burst | io burst */ 

  int ft,tt,it,cw;
        /*finish time | turnaround time | IO time | cpu waiting */

  int remain, start, dur, quan;

  struct __STRUCT_process *prev, *next; 

} process;


process* iniProcess(int);

int myRD(int);
int getCPUB(process *);
int soonest(process *); 

int startCmp(process*, process*);
int durCmp(process*, process*);
int yetCmp(process*, process*);
int statusCmp(process*, process*);
int remainCmp(process*, process*);
int sortPID(process*, process*);

void setQuantum(process *);
void setStatus(process*, int);
void setScheduled (process *,int);
void setStart(process *, int);
void setDur(process*,int);
/*setters set to int directly*/

void updateCW(process *, int);
void updateIOW(process *p, int i);
void updateRemain(process *p, int i);
/*updatters change by int*/

void finish(process *);
void addNext(process *);
void getCandidate(process *);
void run(process *);
void ioWait(process *);

void processPrinter(process *);

#endif
