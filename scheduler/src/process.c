/*                                                                           
** Implementation for Process Structure                       
** Declaration in process.h                                               
** By Qinhua Yan @ NYU Mar 2014                                        
*/

#define PRINT_TRACE(time,pid,state1,state2,dur)

//#define PRINT_TRACE(time,pid,state1,state2,dur)     printf("%d: %d ==> "#state1" - "#state2 "  %d\n", time, pid, dur)



#include "utlist.h"
#include "process.h"
#include "schedule.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int ofs();
extern int cpuTime, rN;
extern schedule *s;
extern process *waitl, *canQuene;
extern int op;
extern IOManager *manager;

int myRD(int burst){
  return 1+(ofs()%burst);
}

int soonest(process *l){
  if (l==NULL){return 0;}
  process *p;
  int t = l->start;
  DL_FOREACH(l,p){
    if (p->start < t){
      t = p->start;
    }
  }
  return t;
}

int startCmp(process *a,process *b){
  return a->start - b->start;
} //soonest first

int durCmp(process *a,process *b){
  return b->dur - a->dur;
} //longest durationg first

int yetCmp(process *a,process *b){
  return a->scheduled - b->scheduled;
}//unscheduled first

int statusCmp(process *a,process *b){
  return a->status - b->status;
}//0 take precedence of 1, reverse normal order=>soonest first
//dangerous, only called when other cmp fail

int remainCmp(process *a,process *b){
  return a->remain - b->remain;
}//shorter remain first

int sortPID(process *a,process *b){
  return a->pid - b->pid;
}//earlier arrival first

void setQuantum(process *cur){
  if (s->algo==4){
    if (cur->quan == 0){
      cur->quan = myRD(cur->cb);
    }
  }
}

int getCPUB(process* p){ 
  //generate cpu burst time 
  //possibly change status to -1 if finished
    int cpu = 0;
    if (s->algo == 4){  //RR
      if (p->quan > s->quan){ //larger than quantum par
	p->quan -= s->quan; //distract from remain cpu burst
	cpu = s->quan;
      }
      else{ 
	cpu = p->quan; 
	p->quan = 0;
      }
    }  
    else{
      cpu = myRD(p->cb);
    } 
    //cpuburst scheduled
    if (cpu >= p->remain){ //can finish within cpu burst
      cpu = p->remain;
      setStatus(p,-1); 
    }
    return cpu;
}

void setStatus(process *p, int i){
  p->status = i;
}
void setScheduled(process *p,int i){
  p->scheduled = i;
}

void setStart(process*p, int i){
  p->start = i;
}

void setDur(process*p,int i){
  p->dur = i;
}

void updateCW(process *p, int now){
  p->cw += now - p->start;
}

void updateIOW(process *p, int i){
  p->it += i;
}

void updateRemain(process *p, int i){
  p->remain -= i;
}

void finish(process *p){
  //change: tt, ft
  //append to finallist
  p->tt = p->tc + p->cw + p->it;
  p->ft = p->tt + p->at;
  DL_APPEND(s->fl,p);
}

void addNext(process * p){
  //add p to waitlist
  //never change anything
    DL_APPEND(waitl, p);
    PRINT_TRACE(p->start,p->pid,IO,READY,0);
}

void getCandidate(process *p){
      DL_DELETE(waitl, p);
      DL_APPEND(canQuene,p);
      rN++;
}

void run(process * p){
  //will change cpuTime because cpu consume
  setScheduled(p,1);
  setStatus(p,0);
  updateCW(p,cpuTime); 
  
  PRINT_TRACE(cpuTime,p->pid,READY,RUN,cpuTime - p->start);
//if (op==1) {printf("%d: %d==> Ready - Run ==> %d\n",cpuTime,p->pid,cpuTime-p->start);}

  int cpu = getCPUB(p); //possibly change status to -1
  updateRemain(p,cpu); 
  cpuTime += cpu;

  setStart(p,cpuTime);
  setDur(p,cpu);

  PRINT_TRACE(cpuTime, p->pid, RUN, IO, cpu);
//if (op==1) {printf("%d: %d==> Run - IO ==> %d remain: %d\n",cpuTime,p->pid,cpu,p->remain);}

}

void ioWait(process * p){
  //doesn't change cpuTime
  if (p->status== -1){
    finish(p);
    return;
  }
  if (s->algo == 4) { 
    if (p->quan > 0){ 
      setStatus(p,0); //unschedulable until as if finish io
      addNext(p);
      return;
    }
    setScheduled(p,0); 
  }

  setStatus(p,0); //unschedulable until time pass
  int iob = myRD(p->io); 
  
  setDur(p, iob);
  updateIOW(p,iob);
  setStart(p,p->start+iob);
  
  PRINT_TRACE(p->start,p->pid,IO,IO,iob);
  //if (op==1) {printf("%d: %d==>Blocked - Ready ==> %d \n",p->start, p->pid,iob);}
  ioUpdate(iob,p->start);
  addNext(p);
}


process* iniProcess(int pid){
  process * p = malloc(sizeof(process));
  p->pid = pid;
  p->status = 0;
  p->scheduled = 0;
  p->cw = 0;
  p->it = 0;
  p->dur = 0;
  p->quan = 0;
  p->prev = NULL;
  p->next = NULL;
  return p;
  /* at, tc, cb, io ->requires input
    remain start initialize according to input */
  /* ft tt to be calculated */
  /* it cw to be incremented */
  /* remain start dur quan change via simulate process */
}




void processPrinter(process *p){
  printf("%04d: %4d %4d %4d %4d | %4d %4d %4d %4d\n",p->pid,p->at,p->tc,p->cb,p->io,p->ft,p->tt,p->it,p->cw);
  return;
}



