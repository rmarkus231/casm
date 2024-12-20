#include <stdio.h>
#include <time.h>
#include "stack.h"
#include <stdlib.h>

void stack_push(struct stack_st* stack ,int v){
	if(stack->len == stack->size){
		resize_stack(stack,stack->size*2);
	}
	stack->arr[stack->len] = v;
	++stack->len;
	return;
};

int resize_stack(struct stack_st* stack, int s){
	int old_size = stack->size;
	if(stack->arr == NULL || stack->size == 0){
		stack->arr = malloc(sizeof(int) * stack->size);
		stack->size = 1;
		if(stack->arr == NULL){
			printf("bad_alloc");
			return 1;
		}
	}else{
		int *tmp_arr = NULL;
		tmp_arr = realloc(stack->arr, s);
		if(tmp_arr == NULL){
			printf("bad_realloc");
			return 1;
		}
		stack->size = s;
		old_size = s;
	}
	for(int i = old_size; i <= stack->size; i++){
			stack->arr[i] = 0;
	}
	return 0;
};

int stack_pop(struct stack_st* stack){
	if(stack->len == 1 && stack->arr != NULL){
		signed int temp = stack->arr[--stack->len];
		stack->size = 0;
		free(stack->arr);
		stack->arr = NULL;
		return temp;
	}
	else if(stack->len < (stack->size/4)){
		resize_stack(stack,(stack->size/4));
	}
	if(!stack_isEmpty(stack)){
		stack->len--;
		signed int temp = stack->arr[stack->len];
		return temp;
	}else{
		return -1;
	}
};

int stack_isEmpty(struct stack_st* stack){
	if(stack->size == 0 || stack->arr == NULL){
		return 1;
	}else{
		return 0;
	}
};

int stack_peek(struct stack_st* stack){
	if(stack_isEmpty(stack)){
		return(stack->arr[stack->arr[stack->len]]);
	}else{
		printf("Empty\n");
		return 0;
	}
};

void stack_print_e(struct stack_st* stack){
	int temp;
	while(stack->len >= 0 && !(stack_isEmpty(stack))){
		temp = stack_pop(stack);
		printf("%d\n",temp);
	}
};
