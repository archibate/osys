; vim: ft=nasm ai

	GLOBAL irq_ents
	GLOBAL move_to_user
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
	popad
	pop dword es
	pop dword ds
	add esp, 8 ; skip irq index & reserved one
	iretd

move_to_user:
	push ebp
	mov ebp, esp

	mov esp, [ebp + 8]
	jmp int_return
