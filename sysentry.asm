; vim: ft=nasm ai

	GLOBAL __syscall_entry
	EXTERN system_call

[SECTION .text]
[BITS 32]

__syscall_entry:
	sub esp, 8
	push dword es
	push dword ds
	pushad
	push esp
	call system_call
	add esp, 4
	popad
	pop dword ds
	pop dword es
	add esp, 8
	iretd
