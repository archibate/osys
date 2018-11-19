; vim: ft=nasm ai

	GLOBAL idle_func

[SECTION .text]
[BITS 32]

idle_func:
	sti
	hlt
	cli
	jmp idle_func
