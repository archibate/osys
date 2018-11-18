; vim: ft=nasm ai

[BITS 32]

_start:
	lea esi, [0x7b00]
	movzx eax, word [esi + 4] ; xsiz
	movzx ecx, word [esi + 6] ; ysiz
	mul ecx
	movzx ecx, word [esi + 10] ; bpp
	mul ecx
	mov ecx, eax
	mov edi, [esi + 0] ; vram
	;mov bx, 3
.again:
	mov eax, ecx
	;shr eax, 1
	and eax, 0x0f
	;div bx
	cld
	stosb
	loop .again
	cli
	hlt
