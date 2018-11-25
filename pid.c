#include <pid.h>
#include <panic.h>
#include <procfs.h>

TCB *proc_table[PID_MAX];

static
int alloc_pid(void)
{
	for (int i = 1; i < PID_MAX; i++) {
		if (!proc_table[i])
			return i;
	}
	panic("Out of PID");
}

int new_proc(TCB *proc)
{
	int pid = alloc_pid();
	proc_table[pid] = proc;
	procfs_new_proc(proc, pid, INODE_RD | INODE_WR);
	return pid;
}
