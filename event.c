#include <event.h>
#include <sched.h>
#include <panic.h>
#include <print.h>


void wait_on
	( EVENT *event
	)
{
	TCB *next = current->next;
	assert(next != current);
	tcb_remove(current);
	tcb_insert_after_ch(current, &event->waiting_head);
	task_run(next);
}


void trig_up
	( EVENT *event
	)
{
	if /*while*/ (event->waiting_head) {
		TCB *next = tcb_shift_forward_ch(&event->waiting_head);
		tcb_insert_after_ch(next, &current);
		task_run(current->next);
	}
}
