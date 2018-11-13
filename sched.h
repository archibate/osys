#pragma once


#include <tcb.h>


extern
TCB *current;

extern
void do_schedule
(void);

extern
void add_task
	( TCB *tcb
	);
