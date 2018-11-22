#include <sysnr.h>

extern int sys_undefined();
extern int sys_exit();
extern int sys_open();
extern int sys_close();
extern int sys_read();
extern int sys_write();
extern int sys_seek();
extern int sys_mmap();
extern int sys_tellsize();
extern int sys_getch();
extern int sys_putch();
extern int sys_fsync();
extern int sys_stexecv1();
extern int sys_waiton();
extern int sys_setbrk();
extern int sys_chdir();
extern int sys_dirfind();

int (*syscall_table[SYSCALL_MAX])() = {
	[_SYS_undefined] = &sys_undefined,
	[_SYS_exit] = &sys_exit,
	[_SYS_open] = &sys_open,
	[_SYS_close] = &sys_close,
	[_SYS_read] = &sys_read,
	[_SYS_write] = &sys_write,
	[_SYS_seek] = &sys_seek,
	[_SYS_mmap] = &sys_mmap,
	[_SYS_tellsize] = &sys_tellsize,
	[_SYS_getch] = &sys_getch,
	[_SYS_putch] = &sys_putch,
	[_SYS_fsync] = &sys_fsync,
	[_SYS_stexecv1] = &sys_stexecv1,
	[_SYS_waiton] = &sys_waiton,
	[_SYS_setbrk] = &sys_setbrk,
	[_SYS_chdir] = &sys_chdir,
	[_SYS_dirfind] = &sys_dirfind,
};
