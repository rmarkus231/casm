#include "mem.h"
#include <stdio.h>
#include "stack.h"

#define debug 0
#define S 30000

struct virt_mem{
	unsigned char mem[S];
	int sp;
	int in_loop;
} vmem = { .sp=0, .in_loop=0};

int mem_add(int i){
	vmem.mem[vmem.sp] += i;
	return vmem.mem[vmem.sp];
};

int mem_move(int i){
	vmem.sp += i;
	return vmem.sp;
};

inline int mem_get(void){if(debug){printf("mem_get\n");}return vmem.mem[vmem.sp];};

int mem_set(char v){
	if(debug){printf("mem_dec\n");}
	int ia = v-0;
	vmem.mem[vmem.sp] = ia;
	return ia;
};

void mem_printDebug(void){
	if(debug){printf("mem_pdbg\n");}
	printf("index: %d ",vmem.sp);
	int lower = vmem.sp;
	int upper = lower + 9;
	if(upper >= S){
		upper -= S;
	}
	printf("mem [%d .. %d]: ",lower,upper);
	int temp;
	for(int i = 0; i<=9;i++){
		printf("%d ",mem_get());
		temp = mem_move(1);
	}
	printf("\n");
	vmem.sp = lower;
};

void mem_startLoop(struct stack_st* s,int i){
	if(debug){printf("mem_startLoop\n");}
	if(vmem.mem[vmem.sp]){
		vmem.in_loop++;
		stack_push(s,i);
	}
}

int mem_endLoop(struct stack_st* s, int i){
	if(debug){printf("mem_endLoop\n");}
	int r = stack_pop(s);
	if((vmem.in_loop) && (vmem.mem[vmem.sp])){
		return --r;
	}else{
		return i;
	}
}
