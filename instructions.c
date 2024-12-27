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
	printf("\t;;instr: %c x %d\n",instr->c,instr->value);
	printf("\tmov eax, %d\n",instr->value);
	printf("\tcall mem_add\n");
}

struct BF_instruction_st* BF_new_increment(int val){
	return BF_generic_constructor(&BF_increment_run,&BF_increment_ASM,val,'+');
}

void BF_move_run(struct BF_instruction_st* instr, int* mem_adr){
	mem_move(instr->value);
	++(*mem_adr);
}


void BF_move_ASM(struct BF_instruction_st* instr, int* mem_adr){
	printf("\t;;instr: %c x %d\n",instr->c,instr->value);
	printf("\tmov eax, %d\n",instr->value);
	printf("\tcall mem_move\n");
	
}

struct BF_instruction_st* BF_new_move(int val){
	return BF_generic_constructor(&BF_move_run, &BF_move_ASM,val,'>');
}

void BF_begin_loop_run(struct BF_instruction_st* instr, int* mem_adr){
	char val = mem_get();
	if(val == 0){
		*mem_adr = *mem_adr + instr->value;
	}else{
		++(*mem_adr);
	}
}

void BF_begin_loop_ASM(struct BF_instruction_st* instr, int* mem_adr){
	printf("\t;;instr: %c\n",instr->c);
	printf("\tcall mem_startLoop\n");
}

struct BF_instruction_st* BF_new_begin_loop(){
	return BF_generic_constructor(&BF_begin_loop_run, &BF_begin_loop_ASM,-1,'[');
}

void BF_end_loop_run(struct BF_instruction_st* instr, int* mem_adr){
	char val = mem_get();
	if(val != 0){
		*mem_adr = *mem_adr - instr->value;
	}else{
		++(*mem_adr);
	}
}

void BF_end_loop_ASM(struct BF_instruction_st* instr, int* mem_adr){
	printf("\t;;instr: %c\n",instr->c);
	printf("\tcall mem_endLoop\n",instr->value);
}

struct BF_instruction_st* BF_new_end_loop(int val){
	return BF_generic_constructor(&BF_end_loop_run, &BF_end_loop_ASM,val,']');
}

void BF_mem_print_debug_run(struct BF_instruction_st* instr, int* mem_adr){
	mem_printDebug();
	++(*mem_adr);
}

void BF_mem_print_debug_ASM(struct BF_instruction_st* instr, int* mem_adr){
	printf("\t;;instr: %c\n",instr->c);
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
	printf("\t;;instr: %c\n",instr->c);
	printf("\tcall mem_set\n");
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
	printf("\t;;instr: %c\n",instr->c);
	printf("\tcall mem_get\n");
}

struct BF_instruction_st* BF_new_mem_get(void){
	return BF_generic_constructor(&BF_mem_get_run, &BF_mem_get_ASM,0,'.');
}

void asm_header(void){
	printf("global main\nsection .data\n"
	"\t bfmem: times 30000 db 0\n"
	"\tinput: db 0\n\toutput: db 0\n"
	"\tbfmem_start: dd 0\n"
	"\nsection .text\n"
	"main:\n"
	"\tmov dword [bfmem_start],bfmem\n"
	"\tpush esi\n"
	"\tpush ebp\n"
	"\tmov esi, bfmem\n\n");
}
void asm_mem_printDebug(void){
	printf("mem_printDebug:\n"
	"\tpush ecx\n"
	"\tpush esi\n"
	"\tpush edx\n"
	"\tpush ebx\n"
	"\tpush eax\n"
	"\tmov ecx, 0\n"
	"\tmov eax, esi\n"
	"\tsub eax, bfmem_start ;;find offset\n"
	"\tnop\n"
	"\tadd eax, 30002 ;always offset 2 \n"
	"\tpush 32\n"
	"\tcall num_to_text\n"
	"\tpush 32 ;space\n"
	"\tpush 120 ;x\n"
	"\tpush 101 ;e\n"
	"\tpush 100 ;d\n"
	"\tpush 110 ;n\n"
	"\tpush 105 ;i\n"
	"\tadd ecx, 7\n"
	"\tcall print\n"
	"\tmov ecx, 0\n\n"
	"\t;;debug index got, now the frame size\n"
	"\tpush 58 ;:\n"
	"\tpush 93 ;]\n"
	"\t;;get index of +9\n"
	"\tmov eax, esi\n"
	"\tsub eax, bfmem_start\n"
	"\tadd eax, 30011\n"
	"\tcall num_to_text\n"
	"\tpush 32\n"
	"\tpush 46\n"
	"\tpush 46 ;.\n"
	"\tpush 32\n"
	"\tmov eax, esi\n"
	"\tsub eax, bfmem_start\n"
	"\tadd eax, 30002\n"
	"\tcall num_to_text\n"
	"\tpush 91 ;[\n"
	"\tpush 32\n"
	"\tpush 109 ;m\n"
	"\tpush 101 ;e\n"
	"\tpush 109\n"
	"\tadd ecx, 11\n"
	"\tcall print\n"
	"\t;;text formating done here\n"
	"\t;;memory dumping starts here\n"
	"\tmov ecx, 0\n"
	"\tpush 10\n"
	"\tmov eax, [esi + 9]\n"
	"\tcall num_to_text\n"
	"\tpush 32\n"
	"\tmov eax, [esi + 8]\n"
	"\tcall num_to_text\n"
	"\tpush 32\n"
	"\tmov eax, [esi + 7]\n"
	"\tcall num_to_text\n"
	"\tpush 32\n"
	"\tmov eax, [esi + 6]\n"
	"\tcall num_to_text\n"
	"\tpush 32\n"
	"\tmov eax, [esi + 5]\n"
	"\tcall num_to_text\n"
	"\tpush 32\n"
	"\tmov eax, [esi + 4]\n"
	"\tcall num_to_text\n"
	"\tpush 32\n"
	"\tmov eax, [esi + 3]\n"
	"\tcall num_to_text\n"
	"\tpush 32\n"
	"\tmov eax, [esi + 2]\n"
	"\tcall num_to_text\n"
	"\tpush 32\n"
	"\tmov eax, [esi + 1]\n"
	"\tcall num_to_text\n"
	"\tpush 32\n"
	"\tmov eax, [esi]\n"
	"\tcall num_to_text\n"
	"\tpush 32\n"
	"\tadd ecx, 11\n"
	"\tcall print\n\n"
	"\tpop eax\n"
	"\tpop ebx\n"
	"\tpop edx\n"
	"\tpop esi\n"
	"\tpop ecx\n"
	"\tret\n\n"
	"num_to_text:\n"
	"\tpop ebp ;;i dont do external func calls so i always know what ebp is and its pushed to stack at start of main\n\n"
	"\ttest eax, eax\n"
	"\tjnz .num_conv\n"
	"\tmov eax, 48 ;;this is incase value is 0\n"
	"\tpush eax\n"
	"\tadd ecx, 1\n"
	"\tpush ebp\n"
	"\tret\n"
	"\t.num_conv:\n"
	"\tmov edx, 0\n"
	"\tmov ebx, 10\n"
	"\tdiv ebx ;;remainder in edx, eax=eax/ebx\n"
	"\tadd edx, 48 ;;ascii 0\n"
	"\tpush edx\n"
	"\tadd ecx, 1\n"
	"\ttest eax,eax\n"
	"\tjnz .num_conv\n"
	"\tpush ebp\n"
	"\tret\n");
}

void asm_mem_endLoop(void){
	printf("mem_endLoop:\n"
	"\tpush eax\n"
	"\txor eax, eax\n"
	"\tmov eax, esi\n"
	"\t;;i chanced stack addressed to be relative\n"
	"\tsub eax, [esi]\n"
	"\tcmp word [eax],0\n"
	"\tjne .mem_endLoop_do\n"
	"\tpop eax\n"
	"\tret\n"
	"\t.mem_endLoop_do:\n"
	"\tsub esi, [esi]\n"
	"\tpop eax\n"
	"\tret\n");
}

void asm_mem_startLoop(void){
	printf("mem_startLoop:\n"
	"\tret\n");
}

void asm_mem_get(void){
	printf("mem_get:\n"
	"\tpush eax\n"
	"\tmov eax, [esi]\n"
	"\tcall putchar\n"
	"\tpop eax\n"
	"\tret\n");
}

void asm_mem_set(void){
	printf("mem_set:\n"
	"\tcall getchar\n"
	"\tmov word [esi],input\n"
	"\tret\n");
}

void asm_mem_add(void){
	printf("mem_add:\n"
	"\t;;adds eax to current esi value\n"
	"\tpush ebx\n"
	"\tmov ebx, [esi]\n"
	"\tadd ebx, eax\n"
	"\tmov [esi], ebx\n"
	"\tpop ebx\n"
	"\tret\n");
}

void asm_mem_move(void){
	printf("mem_move:\n"
	"\t;;moves sp by eax\n"
	"\tadd esi, eax\n"
	"\tcmp esi, [bfmem_start+30002]\n"
	"\tjge .mem_move_oob\n"
	"\tret\n"
	"\t.mem_move_oob:\n"
	"\tsub esi, 30002\n"
	"\tret\n");
}

void asm_getchar(void){
	printf("getchar:\n"
	"\tpush eax\n"
	"\tpush ebx\n"
	"\tpush ecx\n"
	"\tmov eax, 3\n"
	"\tmov ebx, 0\n"
	"\tlea ecx, [input]\n"
	"\tmov edx, 1\n"
	"\tint 0x80\n"
	"\tpop eax\n"
	"\tpop ebx\n"
	"\tpop ecx\n"
	"\tret\n");
}

void asm_print(void){
	printf("print:\n"
	"\tpop ebx\n"
	"\t.print_loop:\n"
	"\tcmp ecx, 0\n"
	"\tje .print_end\n"
	"\tpop eax\n"
	"\tcall putchar\n"
	"\tsub ecx, 1\n"
	"\tjmp .print_loop\n"
	"\t.print_end:\n"
	"\tpush ebx\n"
	"\tret\n");
}

void asm_putchar(void){
	printf("putchar:\n"
	"\tmov [output], al\n"
	"\tpush ebx\n"
	"\tpush ecx\n"
	"\tpush edx\n"
	"\tmov eax, 4\n"
	"\tlea ecx, output\n"
	"\tmov ebx, 1\n"
	"\tmov edx, 1\n"
	"\tint 0x80\n"
	"\tpop edx\n"
	"\tpop ecx\n"
	"\tpop ebx\n"
	"\tret\n");
}
