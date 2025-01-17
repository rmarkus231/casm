MAIN := main.c bfi.c mem.c stack.c instructions.c
MAIN_ASM := brainfuck
EXEC := main
CXX := gcc
CXXFLAGS := -g -std=c11
$(EXEC): $(MAIN) 
	$(CXX) $(CXXFLAGS) -o $(EXEC) $(MAIN) 

r: $(EXEC)
	./$(EXEC) test.txt

c: $(EXEC)
	rm $(EXEC)

asm:
	./$(EXEC) test.txt asm > $(MAIN_ASM).asm
	nasm -g $(MAIN_ASM).asm -felf
	gcc -g -m32 $(MAIN_ASM).o -o brainfuck
	rm $(MAIN_ASM).o
	./$(MAIN_ASM)
