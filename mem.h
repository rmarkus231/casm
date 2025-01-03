#ifndef __MEM_H__
#define __MEM_H__
#include "stack.h"

extern struct virt_mem vmem;

int mem_add(int);
int mem_move(int);
void mem_init();
unsigned char mem_get(void);
int mem_set(char);
void mem_printDebug(void);
int mem_endLoop(struct stack_st*, int);
void mem_startLoop(struct stack_st*, int);

#endif
