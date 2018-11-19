; vim: ft=nasm ai

	GLOBAL irq_ents
	GLOBAL move_to_user
	GLOBAL int_return
	EXTERN tss0
	EXTERN irq_table

[SECTION .rodata]

irq_ents:
%include "gen.irqentable.inc"

[SECTION .text]
[BITS 32]

%include "gen.irqentries.inc"

irq_common:
	sub esp, 4 ; reserved
	push dword ds
	push dword es
	pushad

	mov edx, ss
	mov ds, edx
	mov es, edx

	;push dword [esp + 48] ; old eip
	;push ebp ; fake stack frame
	;mov ebp, esp

	mov eax, [esp + 44] ; irq index
	call [irq_table + eax * 4]

	;add esp, 8

int_return:
	mov dword [tss0 + 4], esp
	add dword [tss0 + 4], 68
	;add dword [tss0 + 4], 60
	;test dword [esp + 52], 3
	;jz .cont
	;add dword [tss0 + 4], 8
;.cont:
	popad
	pop dword es
	pop dword ds
	add esp, 8 ; skip irq index & reserved one
	iretd

move_to_user:
	push ebp
	mov ebp, esp

	mov ecx, 68
	sub esp, ecx
	mov edi, esp
	mov esi, [ebp + 8]
	cld
	rep movsb
	jmp int_return
