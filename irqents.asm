; vim: ft=nasm ai

	GLOBAL irq_ents

[SECTION .rodata]

irq_ents:
%include "gen.irqentable.inc"

[SECTION .text]
[BITS 32]

%include "gen.irqentries.inc"

irq_common:
	pushad
	push dword ds
	push dword es

	mov edx, ss
	mov ds, edx
	mov es, edx

	mov eax, [esp + 40] ; irq index
	call eax

	pop dword es
	pop dword ds
	popad
	add esp, 4 ; skip irq index
	iretd
