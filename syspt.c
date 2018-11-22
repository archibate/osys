#include <pexit.h>
#include <exec.h>
#include <errno.h>
#include <pid.h>
#include <event.h>
#include <panic.h>

int sys_undefined(void)
{
	//panic("undefined system call");
	return -ENOSYS;
}

#define verify_ptr(p) if ((unsigned long) (p) < DMA_END) return -EFAULT
#define verify_pid(pid) if ((pid) < 0 || (pid) > PID_MAX) return -EINVAL
#define verify_eveid(pid) if ((eveid) >= EVE_MAX) return -EINVAL


int sys_wait(int pid, unsigned int eveid)
{
	verify_pid(pid);
	verify_eveid(eveid);

	TCB *proc = get_proc(pid);
	if (!proc)
		return -ESRCH;
	wait_on(&proc->events[eveid]);
	return 0;
}

int sys_stexecv1(const char *name, const char *arg)
{
	return stexecv1(name, arg);
}

void __attribute__((noreturn)) sys_exit(int status)
{
	process_exit(status);
}
