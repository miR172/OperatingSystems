#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifndef MANAGER
#define MANAGER


typedef struct __IOManager{
	int ioTime;
	int enter, leave;
}IOManager;


void iniIOManager();

void ioUpdate(int, int);

void addTime();

#endif
	
