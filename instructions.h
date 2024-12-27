#ifndef __INSTRUCTIONS_H__
#define __INSTRUCTIONS_H__

struct BF_instruction_st{
	void (*run_instr)(struct BF_instruction_st*, int* index);
	void (*printASM)(struct BF_instruction_st*, int* index);
	int value;
	char c;
};

void print_instr(struct BF_instruction_st** ,int);
void BF_increment_run(struct BF_instruction_st*, int*);
void BF_move_run(struct BF_instruction_st*, int*);
void BF_begin_loop_run(struct BF_instruction_st*, int*);
void BF_end_loop_run(struct BF_instruction_st*, int*);
void BF_mem_print_debug_run(struct BF_instruction_st*, int*);
void BF_mem_set_run(struct BF_instruction_st*, int*);
void BF_mem_get_run(struct BF_instruction_st*, int*);

void BF_increment_ASM(struct BF_instruction_st*, int*);
void BF_move_ASM(struct BF_instruction_st*, int*);
void BF_begin_loop_ASM(struct BF_instruction_st*, int*);
void BF_end_loop_ASM(struct BF_instruction_st*, int*);
void BF_mem_print_debug_ASM(struct BF_instruction_st*, int*);
void BF_mem_set_ASM(struct BF_instruction_st*, int*);
void BF_mem_get_ASM(struct BF_instruction_st*, int*);

struct BF_instruction_st* BF_generic_constructor(void*,void*, int,char);
struct BF_instruction_st* BF_new_increment(int);
struct BF_instruction_st* BF_new_move(int);
struct BF_instruction_st* BF_new_begin_loop();
struct BF_instruction_st* BF_new_end_loop(int);
struct BF_instruction_st* BF_new_print_debug(void);
struct BF_instruction_st* BF_new_mem_set(void);
struct BF_instruction_st* BF_new_mem_get(void);

void asm_header(void);
void asm_mem_printDebug(void);
void asm_mem_endLoop(void);
void asm_mem_startLoop(void);
void asm_mem_get(void);
void asm_mem_set(void);
void asm_mem_add(void);
void asm_mem_move(void);
void asm_getchar(void);
void asm_print(void);
void asm_putchar(void);
#endif
