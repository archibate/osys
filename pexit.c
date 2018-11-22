#include <pexit.h>
#include <event.h>
#include <panic.h>
#include <print.h>
#include <uload.h>
#include <sched.h>

static
EVENT eve_zack;

void __attribute__((noreturn)) thread_exit(int val)
{
	trig_up(&current->events[EVE_EXIT], val);

	wait_on(&eve_zack);
	panic("thread exited with %d\n", val);
}

void __attribute__((noreturn)) process_exit(int val)
{
	printf("process_exit(%d)\n", val);
	user_proc_destroy();
	thread_exit(val);
}
