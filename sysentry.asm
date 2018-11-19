; vim: ft=nasm ai

	GLOBAL __syscall_entry
	EXTERN syscall_table
	EXTERN int_return

%include "sysnr.inc"

[SECTION .text]
[BITS 32]

__syscall_entry:
	sub esp, 8
	push dword es
	push dword ds
	pushad
	cmp eax, SYSCALL_MAX
	jnae .cont
	xor eax, eax
.cont:	push ebx
	push edx
	push ecx
	call [syscall_table + eax * 4]
	add esp, 12
	mov [esp + 28], eax
	jmp int_return
