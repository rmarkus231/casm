#include "bfi.h"
#include "mem.h"
#include "stack.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "instructions.h"



enum instructions_e {
    BF_RIGHT      = '>',/* Liiguta mälu indeksit paremale. */
    BF_LEFT       = '<',/* Liiguta mälu indeksit vasakule. */
    BF_INCREASE   = '+',/* Suurenda mälu indeksil olevat väärtust. */
    BF_DECREASE   = '-',/* Vähenda mälu indeksil olevat väärtust. */
    BF_READ       = ',',/* Loe kasutajalt sisendit ja salvesta see mäluindeksile. */
    BF_PRINT      = '.',/* Trüki välja mälu indeksil olev väärtus char'ina. */
    BF_START_LOOP = '[',/* Tsükli algus. */
    BF_END_LOOP   = ']',/* Tsükli lõpp. */
    BF_DEBUG      = '#'    /* Trüki silumise informatsioon. */
};

void interpret(char* prog){
	int i = 0;
	int c;
	char ch;
	struct stack_st STACK= {.len=0,.size=0,.arr=NULL};
	while(prog[i] != 0){
		switch(prog[i]){
			case BF_INCREASE:
				mem_add(1);break;
			case BF_DECREASE:
				mem_add(-1);break;
			case BF_RIGHT:
				mem_move(1);break;
			case BF_LEFT:
				mem_move(-1);break;
			case BF_DEBUG:
				mem_printDebug();break;
			case BF_READ:
				c = getc(stdin);
				if(10 == c){
					c=getc(stdin);
				}
				if(5 == c){ 
					return;
				}
				mem_set(c);break;
			case BF_PRINT:
				ch = mem_get();
				printf("%c",ch);break;
			case BF_START_LOOP:
				mem_startLoop(&STACK,i);break;
			case BF_END_LOOP:
				i = mem_endLoop(&STACK,i);break;
		};
		i++;
	}
	free(STACK.arr);
};

void parse(struct BF_instruction_st** inst_arr, char* prog){
	int i = 0;
	int inst_arr_i = 0; //only incremented when stuff actually added to instructions
	int consecutive_sum = 0;
	int init_sum = 1;
	int begin_index = 0;
	int begin_index_relative = 0;
	struct stack_st STACK= {.len=0,.size=0,.arr=NULL};
	while(prog[i] != 0){
		switch(prog[i]){
			case BF_INCREASE:
				if(init_sum){
					consecutive_sum = 1;
					init_sum = 0;
				}
				if(prog[i+1] != 0 && prog[i+1] == BF_INCREASE){
					consecutive_sum+=1;
					break;
				}else if(prog[i+1] != 0 && prog[i+1] == BF_DECREASE){
					consecutive_sum-=1;
					break;
				}else{
					inst_arr[inst_arr_i] = BF_new_increment(consecutive_sum);
					init_sum = 1;
					++inst_arr_i;
					break;
				}
			case BF_DECREASE:
				if(init_sum){
					consecutive_sum = -1;
					init_sum = 0;
				}
				if(prog[i+1] != 0 && prog[i+1] == BF_INCREASE){
					consecutive_sum+=1;
					break;
				}else if(prog[i+1] != 0 && prog[i+1] == BF_DECREASE){
					consecutive_sum-=1;
					break;
				}else{
					inst_arr[inst_arr_i] = BF_new_increment(consecutive_sum);
					init_sum = 1;
					++inst_arr_i;
					break;
				}
			case BF_RIGHT:
				if(init_sum){
					consecutive_sum = 1;
					init_sum = 0;
				}
				if(prog[i+1] != 0 && prog[i+1] == BF_RIGHT){
					consecutive_sum+=1;
					break;
				}else if(prog[i+1] != 0 && prog[i+1] == BF_LEFT){
					consecutive_sum-=1;
					break;
				}else{
					inst_arr[inst_arr_i] = BF_new_move(consecutive_sum);
					init_sum = 1;
					++inst_arr_i;
					break;
				}
			case BF_LEFT:
				if(init_sum){
					consecutive_sum = -1;
					init_sum = 0;
				}
				if(prog[i+1] != 0 && prog[i+1] == BF_RIGHT){
					consecutive_sum+=1;
					break;
				}else if(prog[i+1] != 0 && prog[i+1] == BF_LEFT){
					consecutive_sum-=1;
					break;
				}else{
					inst_arr[inst_arr_i] = BF_new_move(consecutive_sum);
					init_sum = 1;
					++inst_arr_i;
					break;
				}
			case BF_DEBUG:
				inst_arr[inst_arr_i] = BF_new_print_debug();
				++inst_arr_i;break;
			case BF_READ:
				inst_arr[inst_arr_i] = BF_new_mem_set();
				++inst_arr_i;break;
			case BF_PRINT:
				inst_arr[inst_arr_i] = BF_new_mem_get();
				++inst_arr_i;break;
			case BF_START_LOOP:
				stack_push(&STACK, inst_arr_i);
				inst_arr[inst_arr_i] = BF_new_begin_loop();
				++inst_arr_i;break;
			case BF_END_LOOP:
				begin_index_relative = inst_arr_i - stack_pop(&STACK);
				inst_arr[inst_arr_i] = BF_new_end_loop(begin_index_relative);
				inst_arr[inst_arr_i-begin_index_relative]->value = begin_index_relative;
				++inst_arr_i;break;
		};
		i++;
	}
	free(STACK.arr);
}

void print_instr(struct BF_instruction_st** inst_arr, int len){
	int val;
	char c;
	printf("len: %d",len);
	for(int i = 0; i < len; i++){
		if(inst_arr[i] != NULL){
			val = inst_arr[i]->value;
			c = inst_arr[i]->c;
			printf("%c:%d\n",c,val);
		}
	}
}

void run(struct BF_instruction_st** inst_arr, int len){
	int i = 0;
	void (*run_instr)(struct BF_instruction_st*,int*);
	int val = 0;
	char c = 0;
	while(1){
		if(i < 0 || i >= len){break;}
		if(inst_arr[i] != NULL){
			run_instr = inst_arr[i]->run_instr;
			val = (inst_arr[i]->value);
			c = (inst_arr[i]->c);
			run_instr(inst_arr[i],&i);
			//printf("%d,%c",val,c);
		}else{
			i++;

		}
	}
}

void get_asm(char* prog){
	int len = find_effective_len(prog);
	struct BF_instruction_st** inst_arr = malloc(sizeof(struct BF_instruction_st*) * len + 1);
	void (*inst_incl[8])() = {NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL}; 
	//>:0, +:1, ,:2, .:3, #:4, getc:5, print:6, putchar:7
	if(inst_arr == NULL){
		printf(";;Out of memory, exiting");
	}else{
		parse(inst_arr, prog);
		void (*asm_instr)(struct BF_instruction_st*,int*);
		asm_header();
		struct stack_st STACK= {.len=0,.size=0,.arr=NULL};
		int loop_index = 0;
		//using stack to store the indexes for naming jumps for loops in asm

		for(int i = 0; i < len; i++){
			if(inst_arr[i] != NULL){
				asm_instr = inst_arr[i]->printASM;
				//dont need to add functions for things that are not used :)
				switch(inst_arr[i]->c){
					case BF_INCREASE:
						inst_incl[0] = asm_mem_add;
						break;
					case BF_RIGHT:
						inst_incl[1] = asm_mem_move;
						break;
					case BF_READ:
						inst_incl[2] = asm_mem_set;
						inst_incl[5] = asm_getchar;
						break;
					case BF_PRINT:
						inst_incl[3] = asm_mem_get;
						inst_incl[7] = asm_putchar;
						break;
					case BF_DEBUG:
						inst_incl[4] = asm_mem_printDebug;
						inst_incl[6] = asm_print;
						inst_incl[7] = asm_putchar;
						break;
				}
				if(inst_arr[i]->c == BF_END_LOOP){
					int temp = stack_pop(&STACK);
					asm_instr(inst_arr[i],&temp);
				}else if(inst_arr[i]->c == BF_START_LOOP){
					stack_push(&STACK,loop_index);
					asm_instr(inst_arr[i],&loop_index);
					loop_index += 1;
				}else{
					asm_instr(inst_arr[i],&i);
				}
			}
		}
        	printf("\tpop ebp\n\tpop esi\n\tmov eax, 0\n\tret\n");
		for(int i = 0; i < 8; i++){
			if(inst_incl[i] == NULL){
				continue;
			}
			inst_incl[i]();
		}
		cleanup(inst_arr,len);
	}
}

int find_effective_len(char* prog){
	int len = strlen(prog);
	int eff_len = 0;
	for(int i = 0; i < len; i++){
		if((prog[i] == BF_RIGHT || prog[i] == BF_LEFT) && (prog[i+1] == BF_RIGHT || prog[i+1] == BF_LEFT)){
			continue;
		}else if((prog[i] == BF_INCREASE || prog[i] == BF_DECREASE) && (prog[i+1] == BF_INCREASE || prog[i+1] == BF_DECREASE)){
			continue;
		}else{
			eff_len++;
		}
	}
	return eff_len;
}

void cleanup(struct BF_instruction_st** inst_arr, int len){
	for(int i = 0; i < len;i++){
		free(inst_arr[i]);
	}
	free(inst_arr);
}

void interpret2(char* prog){
	int len = find_effective_len(prog);
	struct BF_instruction_st** inst_arr = malloc(sizeof(struct BF_instruction_st*) * len + 1);
	if(inst_arr == NULL){
		printf("Out of memory, exiting");
	}else{
		parse(inst_arr, prog);
		//toggle to see debug info about characters
		//print_instr(inst_arr,len);
		run(inst_arr, len);
		cleanup(inst_arr,len);
	}
}
