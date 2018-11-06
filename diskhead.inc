; vim: ft=nasm ai

	jmp .entry
	db        0x90
	db        "HARIBOTE"        ; freeparam 启动区的名称可以是任意的字符串（8字节）
	dw        512                ; 每个扇区（sector）的大小（必须为512字节）
	db        1                ; 簇（cluster）的大小（必须为1个扇区）
	dw        1                ; FAT的起始位置（一般从第一个扇区开始）
	db        2                ; FAT的个数（必须为2）
	dw        224                ; 根目录的大小（一般设成224项）
	dw        2880            ; 该磁盘的大小（必须是2880扇区）
	db        0xf0            ; 磁盘的种类（必须是0xf0）
	dw        9                ; FAT的长度（必须是9扇区）
	dw        18                ; 1个磁道（track）有几个扇区（必须是18）
	dw        2                ; 磁头数（必须是2）
	dd        0                ; 不使用分区，必须是0
	dd        2880            ; 重写一次磁盘大小
	db        0,0,0x29        ; 意义不明，固定
	dd        0xffffffff        ; （可能是）卷标号码
	db        "HARIBOTEOS "    ; freeparam 磁盘的名称（11字节）
	db        "FAT12   "        ; 磁盘格式名称（8字节）
	times 18 db 0                ; 先空出18字节
.entry:
