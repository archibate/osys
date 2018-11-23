#pragma once


#include <pcb.h>
#include <evedefs.h>
#include <struct.h>
#include <event.h>

STRUCT(TCB)
{
	TCB *next, *prev;

	KS_REGS *sp;
	unsigned long *pgd;
	const char *name;

	int exit_res;
	EVENT events[EVE_MAX];
};
#define LIST TCB
#define LMETHNAME tcb
#include <list.h>
