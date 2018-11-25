#include <pexit.h>
#include <exec.h>
#include <errno.h>
#include <pid.h>
#include <event.h>
#include <panic.h>
#include <print.h>

int sys_undefined(void)
{
	//panic("undefined system call");
	return -ENOSYS;
}

#define verify_ptr(p) if ((unsigned long) (p) < USER_BEG) return -EFAULT
#define verify_pid(pid) if ((pid) < 0 || (pid) > PID_MAX) return -EINVAL
#define verify_eveid(eveid) if ((eveid) >= EVE_MAX) return -EINVAL


int sys_waiton(int pid, unsigned int eveid)
{
	verify_pid(pid);
	verify_eveid(eveid);

	TCB *proc = get_proc(pid);
	if (!proc)
		return -ESRCH;

	// TODO: assert(proc->state != PROC_WAITING)
	wait_on_ex(&proc->events[eveid], proc);
	return proc->exit_res;
}

int sys_execap(const char *name, const char *arg)
{
	verify_ptr(name);
	verify_ptr(arg);

	return execap(name, arg);
}

void __attribute__((noreturn)) sys_exit(int status)
{
	process_exit(status);
}
