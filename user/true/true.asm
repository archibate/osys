; vim: ft=nasm ai

	GLOBAL _start

[SECTION .text]
[BITS 32]

_start:
	mov ecx, 0
	mov eax, 1
	int 0x80
	cli
	hlt
