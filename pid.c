#include <pid.h>
#include <panic.h>

TCB *proc_table[PID_MAX];

static
int alloc_pid(void)
{
	for (int i = 0; i < PID_MAX; i++) {
		if (!proc_table[i])
			return i;
	}
	panic("Out of PID");
}

int new_proc(TCB *proc)
{
	int pid = alloc_pid();
	proc_table[pid] = proc;
	return pid;
}
