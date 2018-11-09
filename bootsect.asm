; vim: ft=nasm ai

CYLS	equ	9
KERNADR	equ	0x100000
VINFO	equ	0x7c00
VGAMODE equ	0x13
;VBEMODE equ	0x118
;VBEMODE equ	0x103
VBEMODE equ	0x13

	org 0x7c00

[BITS 16]

_start:
%include "diskhead.inc"

	xor ax, ax
	mov ds, ax
	mov es, ax
	mov ss, ax
	mov sp, _start

	mov si, msg_loading
	call print

load_setup:
	mov ax, 0x07e0 ; just after us
	mov es, ax
	mov cx, 0x0002 ; cyln 0, sect 2
	mov dx, 0x0000 ; head 0, drvr 0
.loop:	mov si, 5
.retry:	mov ax, 0x0201
	xor bx, bx
	int 0x13
	jnc .next
	dec si
	jz error
	mov ah, 0x00
	mov dl, 0x00
	int 0x13
	mov ax, 0x0e2f
	int 0x10
	jmp .retry
.next:	mov ax, es
	add ax, 0x0020
	mov es, ax
	inc cl
	cmp cl, 18
	jbe .loop
	mov cl, 1
	inc dh
	cmp dh, 2
	jb .loop
	mov ax, 0x0e2e
	int 0x10
	mov dh, 0
	inc ch
	cmp ch, CYLS
	jb .loop

	mov si, msg_done
	call print
	jmp setup_stage

error:	mov si, msg_err
	call print
halt:	cli
fin:	hlt
	jmp fin

print:
	mov ah, 0x0e
	mov bx, 15
.loop:	lodsb
	test al, al
	jz .done
	int 0x10
	jmp .loop
.done:	ret

msg_loading:
	db "Loading System", 0
msg_done:
	db "done", 13, 10, 0
msg_err:
	db "ERROR", 13, 10, 0

	times 510-($-$$) db 0

	dw 0xaa55

setup_stage:
	xor ax, ax
	mov ds, ax
	mov es, ax

	mov si, msg_setup
	call print

	call set_vbe

enable_32:
	cli
	call delay
	mov al, 0xd1
	out 0x64, al
	call delay
	mov al, 0xdf
	out 0x60, al
	call delay

	lgdt [gdtr0]
	lidt [idtr0]
	mov eax, cr0
	or eax, 0x1
	mov cr0, eax
	jmp .flush
.flush:	mov ax, 0x0010
	mov ds, ax
	mov es, ax
	mov fs, ax
	mov gs, ax
	mov ss, ax

move_kern:
	mov esi, kernel
	mov edi, KERNADR
	mov ecx, (CYLS*18*2-2)*512/4
	call memcpy
	jmp dword 0x0008:KERNADR

memcpy:
	mov eax, [esi]
	add esi, 4
	mov [edi], eax
	add edi, 4
	loop memcpy
	ret

delay:	in al, 0x64
	and al, 0x02
	jnz delay
	ret

%include "setvmode.inc"

	alignb 16
gdt0:   dw 0x0000, 0x0000, 0x0000, 0x0000
	dw 0xffff, 0x0000, 0x9a00, 0x00cf
	dw 0xffff, 0x0000, 0x9200, 0x00cf
	dw 0xffff, 0x0000, 0xfa00, 0x00cf
	dw 0xffff, 0x0000, 0xf200, 0x00cf
gdtr0:	dw $-gdt0-1
	dd gdt0
idtr0:	dw 0
	dd 0

msg_setup:
	db 13, 10, "Setting up protected mode...", 13, 10, 0

vmode_buffer:
	times 512-($-setup_stage) db 0
kernel:
