#include <ontick.h>
#include <sched.h>
#include <print.h>
#include <efifo.h>


static int ticks = 0;


void on_tick
(void)
{
	ticks++;

	/*extern EFIFO *need_flush_efifo;
	if (need_flush_efifo)*/
	//need_flush_efifo = 0;

	do_schedule();
}
