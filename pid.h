#pragma once

#include <piddefs.h>
#include <tcb.h>
#include <sched.h>

extern
TCB *proc_table[PID_MAX];

static inline
TCB *get_proc(int pid)
{
	if (!pid)
		return current;
	else
		return proc_table[pid];
}

int new_proc(TCB *proc);
