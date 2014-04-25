/* Header for intstructions
** Implementation in instruction.c
** Qinhua Yan @ NYU Mar 2014 
*/

#ifndef INSTRUCTION_H
#define INSTRUCTION_H


typedef struct __INSTRUCTION_STRUCT {
  int instN;
  int rw;
  int page;
  struct __INSTRUCTION_STRUCT *prev, *next;
} inst;

void readInstFile(FILE *f);

inst* iniIncode(int, int, int);

void appendTo(inst *);

inst* popInst(inst *thisInst);

void checkInstList();

void printInst(inst *);

#endif 
