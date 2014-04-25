
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "utlist.h"
#include "process.h"
#include "schedule.h"
#include "ioManager.h"



FILE *rf;
int ofs();
int op; /* parameter for trace printer -v */

int cpuTime,  rN; 
/* total CPU time | IO idle time | # process could be scheduled */

process * waitl, *canQuene;
/* process unfinished | tmp quene for schedulable process*/

schedule * s;
/* schedule object to be processed */

IOManager * manager;
/* object keep track of io usage*/

void clean();
void iniGlobal();
void checkPar(FILE *, char, int);


int main(int argc, char *argv[]){

  FILE *fp, *pf;
  pf = NULL;
  rf = NULL;
  char tmp[256];
  int quantum = 0;
  char algo;
  op = 0;

  int i = 1;
  for (; i<argc; i++){
    strcpy(tmp,argv[i]);
    fp = fopen(argv[i],"r");
    
    if (fp == NULL){	
	//fclose(fp);	
      if (argv[i][0]!= '-'){break;}
      switch (argv[i][1]){
	  case 'v':
	    op = 1;  
	    break;
	  case 's':
	    switch (argv[i][2]){
	    case 'R':
	      algo = argv[i][2];
	      char *t = &(argv[i][3]);
	      quantum = atol(t);
	      break;
	    case 'F': case 'L': case 'S':
	      algo = argv[i][2];
	      break;
	    default:
	      break;
	    }
	    break;
      }
    } //no file
   else {
     if (pf==NULL){ pf = fp; }
     else{ rf = fp; }
    } 
  }
	
  checkPar(pf,algo,quantum);/* end iterate cmd arguments check them */
 
  iniGlobal(); /* par no problem: initialize globals*/
  
  if (op==1) {printf("\nalgo: %c\tquan: %d\top: %d\n\n",algo,quantum,op);}
  
  iniSchedule(pf,algo,quantum); /* initialize scheduler with file*/

  simulator(); /*simulate*/
  
  schedulePrinter(); /*print result*/
  
  fclose(pf);
  fclose(rf);
  clean();

  return 0;
}

void iniGlobal(){
  ofs();  // call once to skip first line
  cpuTime = 0;
  rN = 0;
  waitl = NULL;
  iniIOManager();
}

void checkPar(FILE *pf,char algo,int quantum){
  if ((algo!='R') && (algo!='F') && (algo!='L') && (algo!='S')) {
      printf("Need Algorithm -s[FLS|R<num>].\nExit.\n");
      clean();
    }
  
  if ((algo=='R') && (quantum ==0)) {
    printf("Wrong Algorithm: R0\nExit.\n");
    clean();
  }
  
  if ((rf==NULL) || (pf==NULL)){
    printf("Not Enough Files. Please indicate the input and random file.\nExit.\n");
    clean();
  }
}
  

int ofs(){
  int base;
  int n;
  if ( fscanf(rf, "%d",&base) == 1){
    return base;
  }
  rewind(rf);//jump to the head and:
  fscanf(rf,"%d", &n); //skip the first number
  if ( fscanf(rf, "%d", &base) != 1 ){
    printf("Destructive Random File. Please use another random number file.\nExit.\n");
    clean();
  }
  return base;
}

void clean(){
  process *p, *t;
  DL_FOREACH_SAFE(waitl, p, t){
    DL_DELETE(waitl, p);
    free(p);
  }
  DL_FOREACH_SAFE(s->fl, p, t){
    DL_DELETE(s->fl, p);
    free(p);
  }

  free(s);
  s = NULL;

  free(manager);
  manager = NULL;

  exit(0);
}
