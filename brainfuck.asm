global main
global putchar
global mem_add
global mem_get
global mem_set
global mem_startLoop
global mem_endLoop


section .data
	bfmem: times 30000 db 0
	input: db 0

section .text
main:
	push esi
	mov esi, bfmem
	
	push esi
	push 99
	mov ecx, 1
	call print
	add esp, 4

	pop esi
	mov eax, 0
	ret

mem_endLoop:
	push eax
	xor eax, eax
	mov eax, esi
	;;i chanced stack addressed to be relative
	sub eax, [esi]
	cmp word [eax],0
	jne mem_endLoop_do
	pop eax
	ret
mem_endLoop_do:
	sub esi, [esi]
	pop eax
	ret

mem_startLoop:
	;;this really doesnt do anything, all logis is in end_loop
	;;c logic checks in both but idk if thats really neccesary
	ret

mem_get:
	;;prints out value at esi
	push eax
	mov eax, [esi]
	call putchar
	pop eax
	ret

mem_set:
	;;moves value in eax to current esi
	call getchar
	mov word [esi],input
	ret

mem_move:
	;;moves sp by eax
	add esi, eax
	ret

mem_add:
	;;adds eax to current esi value
	push ebx
	mov ebx, [esi]
	add ebx, eax
	mov [esi], ebx
	pop ebx
	ret

getchar:
	;;my own implementation of getc
	push eax
	push ebx
	push ecx
	mov eax, 3
	mov ebx, 0
	lea ecx, [input]
	mov edx, 1
	;;reads 1 byte
	int 0x80
	pop eax
	pop ebx
	pop ecx
	ret
	

print:
	;;prints stack values to stdout using my putchar
	;;uses ecx for string length
	pop eax
	cmp ecx, 0
	jne print_do
	ret
print_do:
	call putchar
	add ecx, -1
	jmp $-6

putchar:
	;;prints eax to stdout
	;;this is my own rendition of putchar btw
	;;comiler explorer just does extern call putchar
	push eax
	mov eax, 4
	pop ecx
	push edx
	mov ebx, 1
	mov edx, 1
	int 0x80
	pop ecx
	pop edx
	pop ebx
	ret
