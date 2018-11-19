; vim: ft=nasm ai

	EXTERN int_return
	GLOBAL exp_ents
%include "gen.expentriext.inc"

[SECTION .rodata]

exp_ents:
%include "gen.expentable.inc"

[SECTION .text]
[BITS 32]

%include "gen.expentries.inc"

exp_common:
	push dword ds
	push dword es
	pushad

	mov edx, ss
	mov ds, edx
	mov es, edx

	;push dword [esp + 48] ; old eip
	;push ebp ; fake stack frame
	;mov ebp, esp

	mov eax, [esp + 40] ; do_exp address
	call eax

	;add esp, 8

	jmp int_return
