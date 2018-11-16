#include <event.h>
#include <sched.h>
#include <panic.h>
#include <print.h>


void wait_on
	( EVENT *event
	)
{
	prrdtsc("W");
	assert(current->next != current);
	tcb_remove(current);
	TCB *next = current->next;
	tcb_insert_after_ch(current, &event->waiting_head);
	task_run(next);
}


void trig_up
	( EVENT *event
	)
{
	prrdtsc("T");
	if /*while*/ (event->waiting_head) {
		TCB *next = tcb_shift_forward_ch(&event->waiting_head);
		tcb_insert_after_ch(next, &current);
	}
	task_run(current->next);
}
