; vim: ft=nasm ai

set_vbe:
	mov di, 0x0800
.chkvbe:
	mov cx, VBEMODE
	mov ax, 0x4f00
	int 0x10
	cmp ax, 0x004f
	jne .novbe
	cmp word [di + 0x04], 0x0200
	jb .novbe
.chkvbemode:
	mov cx, VBEMODE
	mov ax, 0x4f01
	int 0x10
	cmp ax, 0x004f
	jne .novbe
	test word [di], 0x0080
	jz .novbe
.setvbe:
	mov bx, VBEMODE + 0x4000
	mov ax, 0x4f02
	int 0x10
.save:
	mov ax, [di + 0x12]
	mov [VINFO + 4], ax ; screen_x
	mov ax, [di + 0x14]
	mov [VINFO + 6], ax ; screen_y
	mov eax, [di + 0x28]
	mov [VINFO + 0], eax ; video_ram
	mov al, [di + 0x1b]
	mov [VINFO + 10], al ; bits_per_pixel
	mov al, [di + 0x28]
	mov [VINFO + 11], al ; memory_model
	mov ax, VBEMODE
	mov [VINFO + 8], ax ; video_mode
	ret
.novbe:
set_vga13:
	mov ax, VGAMODE
	int 0x10
	mov word [VINFO + 4], 320
	mov word [VINFO + 6], 200
	mov dword [VINFO + 0], 0xa0000
	mov word [VINFO + 8], VGAMODE
	mov byte [VINFO + 10], 8
	mov byte [VINFO + 11], 0
	ret

