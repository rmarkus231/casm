#include "instructions.h"
#include <stdio.h>
#include <stdlib.h>
#include "mem.h"


struct BF_instruction_st* BF_generic_constructor(void* func,void* asm_func,int val,char c){
	struct BF_instruction_st* new = NULL;
	new = malloc(sizeof(struct BF_instruction_st));
	if(new == NULL){
		goto CLEANUP;
	}
	new->value = val;
	new->run_instr = func;
	new->printASM = asm_func;
	new->c = c;
CLEANUP:
	return new;
}

void BF_increment_run(struct BF_instruction_st* instr, int* mem_adr){
	mem_add(instr->value);
	++(*mem_adr);
}

void BF_increment_ASM(struct BF_instruction_st* instr, int* mem_adr){
	printf("\t;;;instr: %c x %d\n",instr->c,instr->value);
	printf("\tpush dword %d\n",instr->value);
	printf("\tcall mem_add\n");
	printf("\tadd esp, 4\n");
}

struct BF_instruction_st* BF_new_increment(int val){
	return BF_generic_constructor(&BF_increment_run,&BF_increment_ASM,val,'+');
}

void BF_move_run(struct BF_instruction_st* instr, int* mem_adr){
	mem_move(instr->value);
	++(*mem_adr);
}


void BF_move_ASM(struct BF_instruction_st* instr, int* mem_adr){
	printf("\t;;;instr: %c x %d\n",instr->c,instr->value);
	printf("\tpush dword %d\n",instr->value);
	printf("\tcall mem_move\n");
	printf("\tadd esp, 4\n");
	
}

struct BF_instruction_st* BF_new_move(int val){
	return BF_generic_constructor(&BF_move_run, &BF_move_ASM,val,'>');
}

void BF_begin_loop_run(struct BF_instruction_st* instr, int* mem_adr){
	int val = mem_get();
	if(instr->value < 0){
		printf(";;;unmatched loop at instr #%d\n",*mem_adr);
	}
	if(val == 0){
		*mem_adr = (instr->value)+1;
	}else{
		++(*mem_adr);
	}
}

void BF_begin_loop_ASM(struct BF_instruction_st* instr, int* mem_adr){
	printf("\t;;;instr: %c\n",instr->c);
	printf("silt_%d:\n",*mem_adr);
	printf("\tcall mem_get\n");
	printf("\tcmp eax, 0\n");
	printf("\tje silt_%d\n",instr->value);
}

struct BF_instruction_st* BF_new_begin_loop(){
	return BF_generic_constructor(&BF_begin_loop_run, &BF_begin_loop_ASM,-1,'[');
}

void BF_end_loop_run(struct BF_instruction_st* instr, int* mem_adr){
	if(instr->value < 0){
		printf("unmatched loop at instr #%d\n",*mem_adr);
	}else{
		*mem_adr = *mem_adr - instr->value;
	}
}

void BF_end_loop_ASM(struct BF_instruction_st* instr, int* mem_adr){
	printf("\t;;;instr: %c\n",instr->c);
	printf("\tjmp silt_%d\n",instr->value);
	printf("silt_%d:\n",*mem_adr);
}

struct BF_instruction_st* BF_new_end_loop(int val){
	return BF_generic_constructor(&BF_end_loop_run, &BF_end_loop_ASM,val,']');
}

void BF_mem_print_debug_run(struct BF_instruction_st* instr, int* mem_adr){
	mem_printDebug();
	++(*mem_adr);
}

void BF_mem_print_debug_ASM(struct BF_instruction_st* instr, int* mem_adr){
	printf("\t;;;instr: %c\n",instr->c);
	printf("\tcall mem_printDebug\n");
}

struct BF_instruction_st* BF_new_print_debug(){
	return BF_generic_constructor(&BF_mem_print_debug_run,&BF_mem_print_debug_ASM, 0,'#');
}

void BF_mem_set_run(struct BF_instruction_st* instr, int* mem_adr){
	char c = getc(stdin);
	if(10 == c){
		c=getc(stdin);
	}
	if(5 == c){ 
		return;
	}
	instr->value= mem_set(c);
	++(*mem_adr);
}


void BF_mem_set_ASM(struct BF_instruction_st* instr, int* mem_adr){
	printf("\t;;;instr: %c\n",instr->c);
	printf("\tcall getchar\n");
	printf("\tcmp eax, 10\n");
	printf("\tje get_%d\n",*mem_adr);
	printf("\tcmp eax, 5\n");
	printf("\tje exit_%d\n",*mem_adr);
	printf("get_%d:\n",*mem_adr);
	printf("\tcall getchar\n");
	printf("\tpush dword eax\n");
	printf("\tcall mem_set\n");
	printf("\tadd esp, 4\n");
	printf("exit_%d:\n",*mem_adr);
}

struct BF_instruction_st* BF_new_mem_set(void){
	return BF_generic_constructor(&BF_mem_set_run, &BF_mem_set_ASM,0,',');
}

void BF_mem_get_run(struct BF_instruction_st* instr, int* mem_adr){
	instr->value = mem_get();
	printf("%d",instr->value);
	++(*mem_adr);
}

void BF_mem_get_ASM(struct BF_instruction_st* instr, int* mem_adr){
	printf("\t;;;instr: %c\n",instr->c);
	printf("\tcall mem_get\n");
	printf("\tpush dword eax\n");
	printf("\tcall putchar\n");
	printf("\tadd esp, 4\n");
}

struct BF_instruction_st* BF_new_mem_get(void){
	return BF_generic_constructor(&BF_mem_get_run, &BF_mem_get_ASM,0,'.');
}

void asm_header(void){
	printf("global main\n"
		"global mem_add\n"
		"global mem_move\n"
		"global mem_get\n"
		"global mem_set\n"
		"global mem_endLoop\n"
		"global mem_startLoop\n"
		"global mem_printDebug\n"
		"section .data\n\t bfmem: times 30000 db 0"
		);
}
