#pragma once


extern
struct TCB *current;

extern
void do_schedule
(void);

extern
void add_task
	( struct TCB *tcb
	);

extern
void task_run
	( struct TCB *next
	);

#include <tcb.h>
