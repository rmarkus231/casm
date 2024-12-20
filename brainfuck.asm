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
	output: db 0
	;;assuming bfmem is cons. loaded
	;;holds bfmem start address
	bfmem_start: dd 0

section .text
main:
	mov dword [bfmem_start],bfmem
	push esi
	mov esi, bfmem
	
	mov eax, 7
	call mem_move
	call mem_printDebug

	pop esi
	mov eax, 0
	ret

mem_printDebug:
	pop edi ;storing return address in edi cuz im doing alot of stack fuckery here
	push ecx
	push esi
	push edx
	push ebx
	push eax

	mov ecx, 0

	sub esi, bfmem_start ;;find offset
	nop
	add esi, 30002 ;always offset 2 
	mov eax, esi

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

	;;debug index got, now the actual info

	pop eax
	pop ebx
	pop edx
	pop esi
	pop ecx
	push edi
	ret
	

num_to_text:
	;;takes number in eax
	;;breks its decimal points down into chars
	;;pushes them onto the stack
	;;adds amount of char pushed to ecx
	pop esi ;;store return in esi

	test eax, eax
	jnz num_conv
	mov edx, 48 ;;this is incase index is 0
	push edx
	add ecx, 1
	jmp num_to_text_end

	num_conv:
	mov edx, 0
	mov ebx, 10
	div ebx ;;remainder in edx, eax=eax/ebx
	add edx, 48 ;;ascii 0
	push edx
	add ecx, 1
	test eax,eax
	jnz num_conv
num_to_text_end:
	push esi
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
print_loop:
	cmp ecx, 0
	je print_end
	pop eax
	call putchar
	sub ecx, 1
	jmp print_loop
print_end:
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
