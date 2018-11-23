; vim: ft=nasm ai

	GLOBAL _start
	EXTERN gview_main

[SECTION .text]
[BITS 32]

_start:
	pop ecx
	push esp
	call gview_main
	mov ecx, eax
	mov eax, 1
	int 0x80
	cli
	hlt
