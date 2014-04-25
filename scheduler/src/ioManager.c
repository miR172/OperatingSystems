#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "ioManager.h"

extern IOManager * manager;

void iniIOManager(){
	manager = malloc(sizeof(IOManager));
	manager->ioTime = 0;
	manager->enter = 0;
	manager->leave = 0;
}

void ioUpdate(int dur, int end){
	//myStart has to be larger than previous enter time
  int myEnd = end;
  int myStart = myEnd -dur;

  if (manager->leave <= myStart){
	addTime();
	manager->enter = myStart;
	manager->leave = myEnd;
	return;
  }
  else if (manager->leave <= myEnd){
	manager->leave = myEnd;
	return;
  }
	//o.w  leave > myEnd>myStart > enter 
}

void addTime(){
  manager->ioTime += manager->leave - manager->enter;
  // printf("io++: %d\n",manager->leave - manager->enter);
}

