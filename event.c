#include <event.h>
#include <sched.h>
#include <panic.h>
#include <print.h>


void wait_on
	( EVENT *event
	)
{
	/*if (event->feeder && event->feeder != current) {
		printf("!!has feeder %p(cur=%p)\n", event->feeder, current);
		tcb_insert_after(event->feeder, current);
	}*/
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
	//event->feeder = current;
	if (event->waiting_head) {
		TCB *next = tcb_shift_forward_ch(&event->waiting_head);
		tcb_insert_after_ch(next, &current);
		assert(current->next == next);
		task_run(current->next);
	}
}
