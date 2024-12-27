global main


section .data
	bfmem: times 30000 db 0
	input: db 0
	output: db 0
	;;assuming bfmem is cons. loaded
	;;holds bfmem start address
	bfmem_start: dd 0

section .text
main:
	mov dword [bfmem_start],bfmem
	push esi
	push ebp
	mov esi, bfmem
	
	mov eax, 10
	call mem_add
	call mem_printDebug

	pop ebp
	pop esi
	mov eax, 0
	ret

mem_printDebug:
	push ecx
	push esi
	push edx
	push ebx
	push eax

	mov ecx, 0

	mov eax, esi
	sub eax, bfmem_start ;;find offset
	nop
	add eax, 30002 ;always offset 2 

	push 32
	call num_to_text	
	push 32 ;space
	push 120 ;x
	push 101 ;e
	push 100 ;d
	push 110 ;n
	push 105 ;i
	add ecx, 7
	call print
	mov ecx, 0

	;;debug index got, now the frame size
	push 58 ;:
	push 93 ;]
	;;get index of +9
	mov eax, esi
	sub eax, bfmem_start
	add eax, 30011
	call num_to_text
	push 32
	push 46
	push 46 ;.
	push 32
	mov eax, esi
	sub eax, bfmem_start
	add eax, 30002
	call num_to_text
	push 91 ;[
	push 32
	push 109 ;m
	push 101 ;e
	push 109
	add ecx, 11
	call print
	;;text formating done here
	;;memory dumping starts here
	;;i can loop because i literally have no registers to store the count here rn, sometimes the dumbest solution is the most efficient
	mov ecx, 0
	push 10
	mov eax, [esi + 9]
	call num_to_text
	push 32
	mov eax, [esi + 8]
	call num_to_text
	push 32
	mov eax, [esi + 7]
	call num_to_text
	push 32
	mov eax, [esi + 6]
	call num_to_text
	push 32
	mov eax, [esi + 5]
	call num_to_text
	push 32
	mov eax, [esi + 4]
	call num_to_text
	push 32
	mov eax, [esi + 3]
	call num_to_text
	push 32
	mov eax, [esi + 2]
	call num_to_text
	push 32
	mov eax, [esi + 1]
	call num_to_text
	push 32
	mov eax, [esi]
	call num_to_text
	push 32
	add ecx, 11

	call print

	pop eax
	pop ebx
	pop edx
	pop esi
	pop ecx
	ret

num_to_text:
	;;takes number in eax
	;;breks its decimal points down into chars
	;;pushes them onto the stack
	;;adds amount of char pushed to ecx
	pop ebp ;;i dont do external func calls so i always know what ebp is and its pushed to stack at start of main

	test eax, eax
	jnz .num_conv
	mov eax, 48 ;;this is incase value is 0
	push eax
	add ecx, 1
	push ebp
	ret
	.num_conv:
	mov edx, 0
	mov ebx, 10
	div ebx ;;remainder in edx, eax=eax/ebx
	add edx, 48 ;;ascii 0
	push edx
	add ecx, 1
	test eax,eax
	jnz .num_conv
	push ebp
	ret

mem_endLoop:
	push eax
	xor eax, eax
	mov eax, esi
	;;i chanced stack addressed to be relative
	sub eax, [esi]
	cmp word [eax],0
	jne .mem_endLoop_do
	pop eax
	ret
	.mem_endLoop_do:
	sub esi, [esi]
	pop eax
	ret

mem_startLoop:
	;;this really doesnt do anything, all logis is in end_loop
	;;c file logic checks in both but idk if thats really neccesary
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
	cmp esi, [bfmem_start+30002]
	;;wraparound check
	jge .mem_move_oob
	ret
	.mem_move_oob:
	sub esi, 30002
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
	;;ebx holds return address
	;;it also prints in reverse
	pop ebx
	.print_loop:
	cmp ecx, 0
	je .print_end
	pop eax
	call putchar
	sub ecx, 1
	jmp .print_loop
	.print_end:
	push ebx
	ret

putchar:
	;;prints eax to stdout
	;;this is my own rendition of putchar btw
	;;comiler explorer just does extern call putchar
	mov [output], al
	push ebx
	push ecx
	push edx
	mov eax, 4
	lea ecx, output
	mov ebx, 1
	mov edx, 1
	int 0x80
	pop edx
	pop ecx
	pop ebx
	ret
