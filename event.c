#include <event.h>
#include <sched.h>
#include <panic.h>
#include <print.h>


void wait_on_ex(EVENT *event, TCB *next)
{
	if (event->trig_count) {
		event->trig_count--;
		return;
	}
	assert(next != current);
	tcb_remove(current);
	tcb_insert_after_ch(current, &event->waiting_head);
	task_run(next);
}


void trig_up(EVENT *event)
{
	event->trig_count++;
	if (event->waiting_head) {
		TCB *next = tcb_shift_forward_ch(&event->waiting_head);
		tcb_insert_before_ch(next, &current);
		assert(current->prev == next);
		task_run(current->prev);
	}
}
