#ifndef __BFI_H__
#define __BFI_H__

#include "instructions.h"
extern struct virt_mem vmem;
void cleanup(struct BF_instruction_st**, int);
void print_instr(struct BF_instruction_st** ,int);
void interpret(char*);
void parse(struct BF_instruction_st**, char*);
void run(struct BF_instruction_st**, int);
void interpret2(char *);
int find_effective_len(char*);
void get_asm(char* prog);

#endif
