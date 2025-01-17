# HOW TO USE


## build with make
- make


## running hello world test case
- this comes with a file called "test.txt", it contains hello world in brainfuck
- "make r" runs this test case
-  to run custom brainfuck use "./main filename" where filename is the file containing brainfuck


## building asm from files
- to build the asm of "test.txt" run "make asm", this will build a file called brainfuck using test.txt
- to build asm of custom brainfuck files do "./main filename asm", this will print the asm to console
- to put the generated asm into a file do "./main filename asm > out.asm" where out.asm is the file you can compile with the asm32 compiler of your choice 
-  you can always change the makefile to use a different file with "make asm"

## common pitfalls
- upon changing code, make sure to "make" before using "make asm" because "make asm" uses the current build of main