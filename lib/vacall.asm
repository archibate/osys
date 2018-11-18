; vim: ft=nasm ai

	GLOBAL vacall

[SECTION .text]
[BITS 32]

vacall:
	push ebp
	mov ebp, esp
	push esi

	mov esi, [ebp + 16]
	mov ecx, [ebp + 12]

	test ecx, ecx
	jz .done

	lea esi, [esi + ecx * 4 - 4]
	std
.again:
	lodsd
	push eax
	loop .again
	cld
.done:

	call [ebp + 8]

	add esp, [ebp + 12]

	leave
	ret
