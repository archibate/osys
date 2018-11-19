; vim: ft=nasm ai

	GLOBAL _start
	EXTERN main

[SECTION .text]
[BITS 32]

_start:
	call main
	mov ecx, eax
	mov eax, 1
	int 0x80
	cli
	hlt
