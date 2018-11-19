; vim: ft=nasm ai

	GLOBAL _start
	EXTERN crt_start

[SECTION .text]
[BITS 32]

_start:
	jmp crt_start
	cli
	hlt
