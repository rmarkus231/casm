#ifndef __STACK_H__
#define __STACK_H__
struct stack_st {
	int len;
	int size;
	int* arr;
};
int stack_pop(struct stack_st*);
void stack_push(struct stack_st*, int);
int stack_isEmpty(struct stack_st*);
int stack_peek(struct stack_st*);
void stack_print_e(struct stack_st*);
int resize_stack(struct stack_st*,int);

#endif
