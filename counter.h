/*
** LRU Algorithms: [laA] use counters
** All MMU programs use counters for summary reports
** Qinhua Yan @ NYU Mar 2014 
*/

#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>

#ifndef _COUNTER_H
#define _COUNTER_H

typedef unsigned long long c64;
typedef unsigned long c32;

void setAdd(c64 *c, c64 setthis);
void shiftAdd(c32 *c, int addthis);

void printBinary(c32 c);

#endif
