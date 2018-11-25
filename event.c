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
	//printf("wait_on: %p=%p\n", event, event->waiting_head);
	tcb_insert_after_ch(current, &event->waiting_head);
	//printf("wait_on_ex: task_run(%p)\n", next);
	task_run(next);
}


void trig_up(EVENT *event)
{
	event->trig_count++;
	//printf("trig_up: %p=%p\n", event, event->waiting_head);
	if (event->waiting_head) {
		TCB *target = tcb_shift_forward_ch(&event->waiting_head);
		tcb_insert_before_ch(target, &current);
		task_run(target);
	}
}
