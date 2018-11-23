#pragma once


#include <tcb.h>

extern
TCB *create_process_ex
	( const char *name
	, int (*proc)()
	, void *arg
	);

#define create_process(proc, arg) create_process_ex("(unnamed)", proc, arg)
