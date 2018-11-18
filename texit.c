#include <texit.h>
#include <event.h>
#include <panic.h>

static
EVENT eve_zack;

void __attribute__((noreturn)) thread_exit(int val)
{
	wait_on(&eve_zack);
	panic("thread exited with %d\n", val);
}
