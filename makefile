MAIN := main.c bfi.c mem.c stack.c instructions.c
MAIN_ASM := brainfuck
MAIN_ASM_temp := template_brainfuck
EXEC := main
CXX := gcc
CXXFLAGS := -g -std=c11
$(EXEC): $(MAIN) 
	$(CXX) $(CXXFLAGS) -o $(EXEC) $(MAIN) 

r: $(EXEC)
	./$(EXEC) 1 2

c: $(EXEC)
	rm $(EXEC)
t: $(EXEC)
	valgrind ./$(EXEC) --leak-check=full >valgrind.log 2>&1

gdb: $(EXEC)
	gdb $(EXEC)

asm:
	./$(EXEC) 1 asm > $(MAIN_ASM).asm
	nasm -g $(MAIN_ASM).asm -felf
	gcc -g -m32 $(MAIN_ASM).o -o brainfuck
	rm $(MAIN_ASM).o
	./$(MAIN_ASM)

asm2:
	nasm -g $(MAIN_ASM_temp).asm -felf
	gcc -g -m32 $(MAIN_ASM_temp).o -o brainfuck
	./brainfuck
