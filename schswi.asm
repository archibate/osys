; vim: ft=nasm ai

	GLOBAL __switch_from_to

[SECTION .text]
[BITS 32]

__switch_from_to:
	pushfd
	push ebp
	push edi
	push esi
	push ebx
	cli
	mov [ecx], esp
	mov eax, [edx + 4]
	mov esp, [edx]
	mov cr3, eax
	pop ebx
	pop esi
	pop edi
	pop ebp
	popfd
	ret
