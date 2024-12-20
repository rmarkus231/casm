MAIN := main.c bfi.c mem.c stack.c instructions.c
MAIN_ASM := brainfuck
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
	#$(CXX) $(CXXFLAGS) -o $(EXEC) $(MAIN) 
	#./$(EXEC) . asm > $(MAIN_ASM).asm
	nasm $(MAIN_ASM).asm -felf
	$(CXX) -g -m32 $(CXXFLAGS) -c mem.c stack.c
	gcc -m32 $(MAIN_ASM).o -o $(MAIN_ASM) mem.o stack.o
	rm $(MAIN_ASM).o
	rm mem.o
	rm stack.o
	./$(MAIN_ASM)

asm2:
	nasm -g $(MAIN_ASM).asm -felf
	gcc -g -m32 $(MAIN_ASM).o -o brainfuck
	./brainfuck
