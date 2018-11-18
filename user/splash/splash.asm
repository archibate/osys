; vim: ft=nasm ai

	GLOBAL _start

[SECTION .text]
[BITS 32]

_start:
	mov edi, 0xfd000000
	mov ecx, 800 * 600
.again:
	mov eax, ecx
	;shr eax, 1
	and eax, 0x0f
	;div bx
	cld
	stosb
	loop .again

	mov eax, 1
	int 0x80
	cli
	hlt
