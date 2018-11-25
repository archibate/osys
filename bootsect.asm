; vim: ft=nasm ai

CYLS	equ	30
KERNADR	equ	0x100000
VINFO	equ	0x7b00
VGAMODE equ	0x13
;VBEMODE equ	0x118
VBEMODE equ	0x103
;VBEMODE equ	0x13

	org 0x7c00

[BITS 16]

_start:
	jmp entry
	nop
	;times 62-($-$$) db 0
bsOEMName	db        "        "        ; 启动区的名称（8字节）
bpbBytesPerSec	dw        512                ; 每个扇区（sector）的大小
bpbSecPerClus	db        1                ; 簇（cluster）的大小
bpbRsvdSecs	dw        2                ; FAT的起始位置
bpbNumFATs	db        2                ; FAT的个数
bpbRootEnts	dw        224               ; 根目录的大小
bpbTotSec16	dw        2880            ; 该磁盘的大小
bpbMedia	db        0xf0            ; 磁盘的种类
bpbFATSize16	dw        9                ; FAT的长度
bpbSecPerTrack	dw        18                ; 1个磁道（track）有几个扇区
bpbNumHeads	dw        2                ; 磁头数
bpbHiddSecs	dd        0                ; 不使用分区数
bpbTotSec32	dd        2880            ; 重写一次磁盘大小
bsDrvNum	db        0x00            ; 磁盘在 BIOS 中的代号
bsReserved1	db        0               ; 保留（必须为0）
bsBootSig	db        0x29            ; 启动签名（意义不明）
bsVolNumber	dd        0xffffffff      ; 磁盘的卷号码
bsVolLabel	db        "           "    ; 磁盘的卷标（11字节）
bsFSysType	db        "FAT12   "       ; 磁盘格式名称（8字节）

entry:
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
	movzx dx, byte [bsDrvNum] ; head 0, drvr 0
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
	mov ah, 0x00
	int 0x16
	int 0x19
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
	db 13, 10, "ERROR occurred, Press any key to reset", 13, 10, 0

	times 510-($-$$) db 0

	dw 0xaa55

setup_stage:
	xor ax, ax
	mov ds, ax
	mov es, ax

	mov si, msg_setup
	call print

search_kern:
	movzx ax, byte [bpbNumFATs]
	imul ax, [bpbFATSize16]
	add si, [bpbHiddSecs]
	imul ax, 512 ; sect size
	mov si, ax
	add si, FAT ; now *si = root dir ents
	mov bx, [bpbRootEnts]
	imul bx, 0x20 ; ent size
	add bx, si ; now *bx = root ents end
	push bx
.again:
	lea di, [kern_name]
	mov cx, 11
	push si
	cld
	repe cmpsb
	je .found
	pop si
	add si, 0x20 ; next entry
	cmp si, bx ; root ents end
	jb .again
	mov si, msg_nofound
	call print
	jmp error
.found:
	mov si, msg_done
	call print

enable_32:
	call set_vbe
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
load_kern:
	pop si
	movzx ecx, byte [bpbSecPerClus]
	imul ecx, 512 ; sect size
	mov ebp, [si + 0x1c] ; file size
	mov ax, [si + 0x1a] ; begin cluster
	pop si ; root ents end
	movzx esi, si
	sub esi, ecx ; eeee...
	sub esi, ecx ; - two clus
	mov edi, KERNADR
.begin:
	and eax, 0xfff
	cmp ax, 0xff7
	je error
	ja .done
	push eax
	push esi
	imul eax, ecx
	add esi, eax
	push ecx
	;push edi
	call memcpy
	;pop edi
	pop ecx
	;add edi, ecx
	pop esi
	pop eax
	mov bp, ax
	mov bx, 3
	mul bx
	shr ax, 1
	lea bx, [FAT]
	add bx, ax
	mov ax, [bx]
	test bp, 1
	jz .begin
	shr ax, 4
	jmp .begin
.done:
	jmp dword 0x0008:KERNADR

memcpy:
	mov eax, [esi]
	add esi, 4
	mov [edi], eax
	add edi, 4
	sub ecx, 4
	jnz memcpy
	ret

delay:	in al, 0x64
	and al, 0x02
	jnz delay
	ret

%include "setvmode.inc"

kern_name:
	db "KERNEL  BIN" ; kernel name in root entry, must be 11 bytes

	alignb 16
gdt0:   dw 0x0000, 0x0000, 0x0000, 0x0000
	dw 0xffff, 0x0000, 0x9a00, 0x00cf
	dw 0xffff, 0x0000, 0x9200, 0x00cf
	dw 0xffff, 0x0000, 0xfa00, 0x00cf
	dw 0xffff, 0x0000, 0xf200, 0x00cf
	dw 0x0000, 0x0000, 0x0000, 0x0000
	dw 0x0000, 0x0000, 0x0000, 0x0000
gdtr0:	dw $-gdt0-1
	dd gdt0
idtr0:	dw 0
	dd 0

msg_setup:
	db "Seting up kernel...", 0
msg_nofound:
	db 13, 10, "Kernel not found!", 0

vmode_buffer:
	times 512*8-($-$$) db 0
FAT:
