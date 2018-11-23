#include <print.h>
#include <errno.h>
#include <upcb.h>
#include <pid.h>
#include <map.h>

#define verify_ptr(p) if ((unsigned long) p < USER_BEG) return -EFAULT

int sys_map_upcb(int pid, void *p, unsigned int mattr)
{
	verify_ptr(p);
	p = (void *) (PGMASK & (unsigned long) p);

	TCB *proc = get_proc(pid);
	if (!proc)
		return -ESRCH;

	printf("sys_map_upcb(%d[@%p], %p, %#x)\n", pid, proc, p, mattr);

	mattr &= PG_W;
	mattr |= PG_P | PG_U;
	map((unsigned long)p, (unsigned long)UPCB_OF(proc->pgd) | mattr);

	return 0;
}
