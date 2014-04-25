/*                                                              
** Header for Schedule  Structure
** Implementation in schedule.c                                 
** By Qinhua Yan @ NYU Mar 2014                                 
*/


#include "utlist.h"
#include "process.h"
#include "schedule.h"
#include "ioManager.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int ofs();
int myRD();
void clean(); 

extern int cpuTime, rN;
extern process *waitl, *canQuene;
extern schedule *s;
extern int op;
extern IOManager *manager;


int fSort(process *a, process *b){
  int t1 = startCmp(a,b);
  if (t1 == 0){
    t1 = yetCmp(a,b);
    if (t1==0){
      t1 = durCmp(a,b);
      if (t1==0){
	t1 = statusCmp(a,b);
	if (t1==0){
	  t1 = sortPID(a,b);
	}}}}
  return t1;
}

int lSort(process *a, process *b){
  int t1 = 0 - startCmp(a,b);
  if (t1==0){
    t1 = 0 -yetCmp(a,b);
    if (t1==0){
      t1 = 0 - durCmp(a,b);
      if (t1==0){
	t1 = statusCmp(a,b);
	if (t1==0){
	  t1 = 0 - sortPID(a,b);
	}}}}
  return t1;
}

int sSort(process *a, process *b){
  int t1 = remainCmp(a,b);
  if (t1==0){
    t1 = startCmp(a,b);
    if (t1==0){
      t1 = yetCmp(a,b);
      if (t1==0){
	t1 = durCmp(a,b);
	if (t1==0){
	  t1 = statusCmp(a,b);
	  if (t1==0){
	    t1 = sortPID(a,b);
	  }}}}}
  return t1;
}
      

int rSort(process *a, process *b){
  int t1 = startCmp(a,b);
  if (t1==0){
    t1 = yetCmp(a,b);
    if (t1==0){
      t1 = durCmp(a,b);
      if (t1==0){
	t1 = sortPID(a,b);
      }}}
  return t1;
}


void simulator(){  
  process *p;
  while (waitl!=NULL) {
    cpuWait(); //change CPUtime for IDLE
    if (canQuene == 0){ continue; }
    p = getNext();
    setQuantum(p);
    run(p); //change CPUtime for consuming
    ioWait(p);
  }
  ioUpdate(0, manager->leave);
  DL_SORT(s->fl,sortPID);
  return;
}

void cpuWait(){
  //will change cpuTime because cpu idle
  //update rN and canQuene
  process *p, *t;
  int pTime = soonest(waitl);
  rN=0;
  canQuene = NULL;

  if (pTime > cpuTime){
    if(op == 1) {printf("\n\n\tCPU IDLE %d-%d\n\n",cpuTime, pTime);}
    cpuTime = pTime;  
  }

  DL_FOREACH_SAFE(waitl, p, t){
    if (p->status == -1){ 
      DL_DELETE(waitl,p);
      finish(p);
    }
    if (p->status > 0){ getCandidate(p); }
    else if (p->start <= cpuTime){ getCandidate(p);} 
  }
}



void returnCandidate(){
  if (canQuene == NULL){return;}
  process* p, *t;
  DL_FOREACH_SAFE(canQuene,p,t){
    p->status++;
    DL_DELETE(canQuene,p);
    DL_PREPEND(waitl,p);
  }
  rN = 0;
  canQuene =NULL;
}

process* getNext(){
  //don't change cpuTime, don't change anyfield in process
  //don't worry about no process to get
  //call the right algo with returns a process
  //call returnCandidate to put other process back to waitl with ready status marked
  //the returned process is deleted from waitl

  process *p;
  
  switch (s->algo){
  case 1: //FCFS
    DL_SORT(canQuene, fSort);
    break;
  case 2: //LCFS
    DL_SORT(canQuene, lSort);
    break;
  case 3: //Shortest Remain
    DL_SORT(canQuene, sSort);
    break;
  case 4: // RR
    DL_SORT(canQuene, rSort);
    break;
  }

  p = canQuene;
  DL_DELETE(canQuene,p);
  returnCandidate();

  return p;
}


void iniSchedule(FILE *f, char algo,int quantum){ 
  s = malloc(sizeof(schedule));

  int rp = 0;
  process * l = NULL;
  process * cur;
  
  switch (algo){
    case 'F': 
      s->algo = 1; break;
    case 'L':
      s->algo = 2; break;
    case 'S':
      s->algo = 3; break;
    case 'R':
      s->algo = 4; break;
  }

  while(!feof(f)) {
    cur = iniProcess(rp);
    if (fscanf(f,"%d", &(cur->at)) == 1 ){
      if (fscanf(f,"%d", &(cur->tc)) == 1 ){
	if (fscanf(f,"%d", &(cur->cb)) == 1 ){
	  if (fscanf(f,"%d", &(cur->io)) == 1 ){
	    rp++;
	    cur->remain = cur->tc;
	    cur->start = cur->at;
	    DL_APPEND(l,cur); //(head->0->1->2->3)
	  }}}}
    else{
      break;
    }
  }
    s->pN = rp;
    s->quan = quantum;
    s->fl = NULL;
    waitl = l;
    
    if (rp==0){
      printf("# Process To Be Scheduled = %d. Try Another Input.\nExit.\n",rp);
      clean();
    }

  return;
}


void schedulePrinter(){
  
  char *algos[4] = {"FCFS","LCFS","SJF","RR"}; 
  printf("%s",algos[s->algo -1]);
  if (s->algo == 4 ){
    printf(" %d\n",s->quan);
  }
  else{
    printf("\n");
  }

  int totalCPU = 0;
  int totalTT = 0;
  int totalCW = 0;
  double pctCPU, pctIO, avgTT, avgCW, pTime;  

  process *p;
  DL_FOREACH(s->fl, p){
    totalCPU += p->tc;
    totalTT += p->tt;
    totalCW += p->cw;
    processPrinter(p);
  } //process printed

  pctCPU = totalCPU*100.00/cpuTime;
  pctIO =  (manager->ioTime)*100.00/cpuTime;
  avgTT = totalTT*1.0/s->pN;
  avgCW = totalCW*1.0/s->pN;
  pTime = s->pN*100.00/cpuTime;
  
  printf("SUM: %d %.2lf %.2lf %.2lf %.2lf %.3lf\n", cpuTime, pctCPU, pctIO, avgTT, avgCW, pTime );
  //  printf("\nioTime=%d time=%d\n",ioTime,time);
}






