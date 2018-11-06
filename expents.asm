; vim: ft=nasm ai

	GLOBAL exp_ents
%include "gen.expentriext.inc"

[SECTION .rodata]

exp_ents:
%include "gen.expentable.inc"

[SECTION .text]
[BITS 32]

%include "gen.expentries.inc"

exp_common:
	pushad
	push dword ds
	push dword es

	mov edx, ss
	mov ds, edx
	mov es, edx

	mov eax, [esp + 40] ; do_exp address
	call eax

	pop dword es
	pop dword ds
	popad
	add esp, 8 ; skip do_exp address & error code
	iretd
