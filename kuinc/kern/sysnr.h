#pragma once

#define SYSCALL_MAX (17+1)
#define _SYS_undefined 0
#define _SYS_exit 1
#define _SYS_open 2
#define _SYS_close 3
#define _SYS_read 4
#define _SYS_write 5
#define _SYS_seek 6
#define _SYS_mmap 7
#define _SYS_tellsize 8
#define _SYS_getch 9
#define _SYS_putch 10
#define _SYS_fsync 11
#define _SYS_dirfind 12
#define _SYS_chdir 13
#define _SYS_fork 14
#define _SYS_execap 15
#define _SYS_waiton 16
#define _SYS_setbrk 17
