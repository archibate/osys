#include "ontick.h"
#include "sched.h"
#include "print.h"


static volatile int ticks = 0;


void on_tick
(void)
{
	//printf("tick %d\n", ticks);
	ticks++;

	do_schedule();
}
