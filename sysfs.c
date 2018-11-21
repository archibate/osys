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

#define verify_fd(fd) if ((fd) < 0 || (fd) > FILES_MAX || !current->pcb->files[(fd)]) return -E_BAD_FD
#define verify_ptr(p) if ((unsigned long) p < DMA_END) return -E_SEG_FL
#define verify_mappable_ptr(p) if ((unsigned long) (p) < USER_BEG || (unsigned long) (p) > USER_STACK_BEG) return -E_SEG_FL

static
int allocate_fd(void)
{
	for (int fd = 0; fd < FILES_MAX; fd++) {
		if (!current->pcb->files[fd]) {
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
		return -E_OO_MAX;
	}

	current->pcb->files[fd] = kmalloc_for(FILE);

	int res = open(current->pcb->files[fd], name, oattr);

	if (res < 0) {
		kfree(current->pcb->files[fd]);
		fd = res;
	}

	return fd;
}

int sys_dirfind(DIRENT *res, int fd, const char *name)
{ // here to go!!!
	verify_fd(fd);
	verify_ptr(res);
	verify_ptr(name);

	DIRENT *de = dirfind(current->pcb->files[fd], name);
	if (!de)
		return -E_NO_SRCH;

	memcpy(res, de, sizeof(DIRENT));

	return 0;
}

int sys_close(int fd)
{
	verify_fd(fd);

	close(current->pcb->files[fd]);
	kfree(current->pcb->files[fd]);

	current->pcb->files[fd] = 0;

	return 0;
}

long sys_tellsize(int fd)
{
	verify_fd(fd);

	return current->pcb->files[fd]->f_size;
}

int sys_mmap(int fd, void *p, size_t size, unsigned int mattr)
{
	verify_fd(fd);
	verify_mappable_ptr(p);

	mattr |= MMAP_USR;

	return mmap(current->pcb->files[fd], p, size, mattr);
}

int sys_read(int fd, void *buf, size_t size)
{
	verify_fd(fd);
	verify_ptr(buf);

	return read(current->pcb->files[fd], buf, size);
}

int sys_write(int fd, const void *buf, size_t size)
{
	verify_fd(fd);
	verify_ptr(buf);

	return write(current->pcb->files[fd], buf, size);
}

long sys_seek(int fd, long offset, int whence)
{
	verify_fd(fd);

	return seek(current->pcb->files[fd], offset, whence);
}

int sys_putch(int fd, unsigned char ch)
{
	verify_fd(fd);

	return putch(current->pcb->files[fd], ch);
}

unsigned int sys_getch(int fd)
{
	verify_fd(fd);

	return getch(current->pcb->files[fd]);
}

int sys_fsync(int fd)
{
	verify_fd(fd);

	return fsync(current->pcb->files[fd]);
}

int sys_chdir(const char *path)
{
	verify_ptr(path);

	return chdir(path);
}
