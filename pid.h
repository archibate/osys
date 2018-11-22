#pragma once

#include <piddefs.h>
#include <tcb.h>

extern
TCB *proc_table[PID_MAX];

static inline
TCB *get_proc(int pid)
{
	return proc_table[pid];
}

int new_proc(TCB *proc);
