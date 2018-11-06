; vim: ft=nasm ai

set_vga13:
	mov ax, 0x0013
	int 0x10
	mov cx, 320
	mov dx, 200
	mov ebx, 0xa0000
	mov ax, 0x0013
	ret

set_vbe103:
	lea di, [vmode_buffer]
	mov bx, 0x4103
	mov ax, 0x4f02
	int 0x10
	mov cx, [di + 0x12]
	mov dx, [di + 0x02]
	mov ebx, [di + 0x28]
	mov ax, 0x0103
	ret
