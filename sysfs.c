#include <uregs.h>
#include <print.h>
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

int sys_open(const char *name, unsigned int oattr)
{
	verify_ptr(name);

	int res = -E_OO_MAX, fd;

	for (fd = 0; fd < FILES_MAX; fd++) {
		if (!current->pcb->files[fd]) {
			goto got_fd;
		}
	}
	goto out;
got_fd:	res = 0;
	current->pcb->files[fd] = kmalloc_for(FILE);
	res = open(current->pcb->files[fd], name, oattr);
	if (res)
		goto out_free;
	res = fd;

	goto out;

out_free:
	kfree(current->pcb->files[fd]);
out:
	return res;
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

int sys_flush(int fd)
{
	verify_fd(fd);

	return flush(current->pcb->files[fd]);
}
