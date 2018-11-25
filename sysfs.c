#include <uregs.h>
#include <print.h>
#include <memory.h>
#include <sched.h>
#include <kmalloc.h>
#include <vfs.h>
#include <panic.h>
#include <memlay.h>
#include <pexit.h>
#include <errno.h>
#include <umemlay.h>
#include <kern/sysapi.h>

#define verify_fd(fd) if ((fd) < 0 || (fd) > FILES_MAX || !curr_upcb.files[(fd)]) return -EBADFD
#define verify_ptr(p) if ((unsigned long) p < USER_BEG) return -EFAULT
#define verify_mappable_ptr(p) if ((unsigned long) (p) < USER_BEG || (unsigned long) (p) > USER_STACK_BEG) return -EFAULT

static
int allocate_fd(void)
{
	for (int fd = 0; fd < FILES_MAX; fd++) {
		if (!curr_upcb.files[fd]) {
			return fd;
		}
	}
	return -1;
}

int sys_open(const char *name, unsigned int oattr)
{
	verify_ptr(name);

	int fd = allocate_fd();
	if (fd == -1) {
		return -EMFILE;
	}

	curr_upcb.files[fd] = kmalloc_for(FILE);

	int res = open(curr_upcb.files[fd], name, oattr);

	if (res < 0) {
		kfree(curr_upcb.files[fd]);
		curr_upcb.files[fd] = 0;
		fd = res;
	}

	return fd;
}

int sys_dirfind(int fd, U_DIRENT *res, const char *name)
{
	verify_fd(fd);
	verify_ptr(res);
	verify_ptr(name);

	DIRENT *de = dirfind(curr_upcb.files[fd], name);
	if (!de)
		return -ENOENT;

	memcpy(res, &de->e_ude, sizeof(DIRENT));

	return 0;
}

int sys_readdir(int fd, U_DIRENT *res)
{
	verify_fd(fd);
	verify_ptr(res);

	DIRENT *de = readdir(curr_upcb.files[fd]);
	if (!de)
		return -ENOENT;

	memcpy(res, &de->e_ude, sizeof(U_DIRENT));

	return 0;
}

int sys_rewinddir(int fd)
{
	verify_fd(fd);

	return rewinddir(curr_upcb.files[fd]);
}

int sys_close(int fd)
{
	verify_fd(fd);

	close(curr_upcb.files[fd]);
	kfree(curr_upcb.files[fd]);

	curr_upcb.files[fd] = 0;

	return 0;
}

long sys_tellsize(int fd)
{
	verify_fd(fd);

	return curr_upcb.files[fd]->f_size;
}

int sys_mmap(int fd, void *p, size_t size, unsigned int mattr)
{
	verify_fd(fd);
	verify_mappable_ptr(p);

	mattr |= MMAP_USR;

	return mmap(curr_upcb.files[fd], p, size, mattr);
}

int sys_read(int fd, void *buf, size_t size)
{
	verify_fd(fd);
	verify_ptr(buf);

	return read(curr_upcb.files[fd], buf, size);
}

int sys_write(int fd, const void *buf, size_t size)
{
	verify_fd(fd);
	verify_ptr(buf);

	return write(curr_upcb.files[fd], buf, size);
}

long sys_seek(int fd, long offset, int whence)
{
	verify_fd(fd);

	return seek(curr_upcb.files[fd], offset, whence);
}

int sys_putch(int fd, unsigned char ch)
{
	verify_fd(fd);

	return putch(curr_upcb.files[fd], ch);
}

unsigned int sys_getch(int fd)
{
	verify_fd(fd);

	return getch(curr_upcb.files[fd]);
}

int sys_fsync(int fd)
{
	verify_fd(fd);

	return fsync(curr_upcb.files[fd]);
}

int sys_chdir(const char *path)
{
	verify_ptr(path);

	return chdir(path);
}
