#include <uregs.h>
#include <sched.h>
#include <kmalloc.h>
#include <vfs.h>
#include <panic.h>
#include <memlay.h>
#include <mkproc.h>
#include <texit.h>
#include <errno.h>
#include <umemlay.h>
#include <kern/sysapi.h>

int __attribute__((noreturn)) sys_undefined(void)
{
	panic("undefined system call");
}

void __attribute__((noreturn)) sys_exit(int status)
{
	thread_exit(status);
}

#define verify_fd(fd) if ((fd) < 0 || (fd) > FILES_MAX || !current->pcb->files[(fd)]) return -E_INVL_ARG
#define verify_ptr(p) if ((unsigned long) p < DMA_END) return -E_ACC_VIOL
#define verify_mappable_ptr(p) if ((unsigned long) (p) < USER_BEG || (unsigned long) (p) > USER_STACK_BEG) return -E_ACC_VIOL

int sys_open(const char *name, unsigned int oattr)
{
	verify_ptr(name);

	int res = 0, fd;

	for (fd = 0; fd < FILES_MAX; fd++) {
		if (!current->pcb->files[fd]) {
			res = -E_OO_MAX;
			goto got_fd;
		}
	}
	goto out;
got_fd:
	current->pcb->files[fd] = kmalloc_for(FILE);
	res = open(current->pcb->files[fd], name, oattr);
	if (res)
		goto out_free;

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

	return mmap(current->pcb->files[fd], p, size, mattr);
}

int sys_read(int fd, char *buf, size_t size)
{
	verify_fd(fd);
	verify_ptr(buf);

	return read(current->pcb->files[fd], buf, size);
}

int sys_write(int fd, const char *buf, size_t size)
{
	verify_fd(fd);
	verify_ptr(buf);

	return write(current->pcb->files[fd], buf, size);
}
