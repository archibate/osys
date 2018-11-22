; vim: ft=nasm ai

	GLOBAL _start
	EXTERN __crt_start

[SECTION .text]
[BITS 32]

_start:
	call __crt_start
	cli
	hlt
