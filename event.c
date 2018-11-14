#include <event.h>
#include <sched.h>


void wait_on
	( EVENT *event
	)
{
	tcb_remove_ch(&current);
	TCB *next = current->next;
	tcb_insert_after_ch(current, &event->waiting_head);
	task_run(next);
}


void trig_up
	( EVENT *event
	)
{
	TCB *next = tcb_shift_forward_ch(&event->waiting_head);
	tcb_insert_after_ch(next, &current);
	task_run(next);
}


void post_on
	( EVENT *event_inc
	, EVENT *event_dec
	)
{
	TCB *next = tcb_shift_forward_ch(&event_inc->waiting_head);
	tcb_insert_after_ch(next, &current);
	task_run(next);
}
