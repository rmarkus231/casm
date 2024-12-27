global main
section .data
	 bfmem: times 30000 db 0
	input: db 0
	output: db 0
	bfmem_start: dd 0

section .text
main:
	mov dword [bfmem_start],bfmem
	push esi
	push ebp
	mov esi, bfmem

	;;instr: + x 2
	mov eax, 2
	call mem_add
	;;instr: #
	call mem_printDebug
	pop ebp
	pop esi
	mov eax, 0
	ret
mem_add:
	;;adds eax to current esi value
	push ebx
	mov ebx, [esi]
	add ebx, eax
	mov [esi], ebx
	pop ebx
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
print:
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
