#include <ontick.h>
#include <sched.h>
#include <print.h>
#include <efifo.h>


extern EFIFO *need_flush_efifo;
static volatile int ticks = 0;


void on_tick
(void)
{
	//printf("tick %d\n", ticks);
	ticks++;

	if (need_flush_efifo)
		efifo_flush(need_flush_efifo);
	need_flush_efifo = 0;

	do_schedule();
}
