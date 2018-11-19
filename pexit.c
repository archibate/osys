#include <pexit.h>
#include <event.h>
#include <panic.h>
#include <print.h>
#include <map.h>

static
EVENT eve_zack;

void __attribute__((noreturn)) thread_exit(int val)
{
	wait_on(&eve_zack);
	panic("thread exited with %d\n", val);
}

void __attribute__((noreturn)) process_exit(int val)
{
	printf("process_exit(%d)\n", val);
	unmap_free_psm_non_global_pages();
	thread_exit(val);
}
