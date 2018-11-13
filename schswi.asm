; vim: ft=nasm ai

	GLOBAL __switch_from_to

[SECTION .text]
[BITS 32]

__switch_from_to:
	push ebp
	push edi
	push esi
	push ebx
	mov [ecx], esp
	mov esp, [edx]
	pop ebx
	pop esi
	pop edi
	pop ebp
	ret
