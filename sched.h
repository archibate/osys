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

extern
void task_run
	( TCB *next
	);

void run_pcb
	( PCB *next
	);
