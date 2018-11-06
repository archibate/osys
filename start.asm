; vim: ft=nasm ai

	GLOBAL _start
	EXTERN main

[SECTION .text]
[BITS 32]

	; org 0x100000
_start:
	mov eax, 0x0010
	mov ss, eax
	mov esp, 0x200000
	xor ebp, ebp

	call main

	cli
	hlt
