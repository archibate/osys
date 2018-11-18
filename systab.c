#include <sysnr.h>

extern int sys_undefined();
extern int sys_exit();
extern int sys_open();
extern int sys_close();
extern int sys_read();
extern int sys_write();
extern int sys_mmap();
extern int sys_tellsize();

int (*syscall_table[SYSCALL_MAX])() = {
	[_SYS_undefined] = &sys_undefined,
	[_SYS_exit] = &sys_exit,
	[_SYS_open] = &sys_open,
	[_SYS_close] = &sys_close,
	[_SYS_read] = &sys_read,
	[_SYS_write] = &sys_write,
	[_SYS_mmap] = &sys_mmap,
	[_SYS_tellsize] = &sys_tellsize,
};
