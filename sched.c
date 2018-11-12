#include <sched.h>
#include <schswi.h>


TCB *current;


static char kstk0[1024];

static TCB tcb0 = {
	.list = STNODE(&tcb0),
	.sp = (KS_REGS *) (kstk0 + sizeof(kstk0) - sizeof(KS_REGS)),
};

void init_sched
(void)
{
	current = &tcb0;
}


void switch_to
	( TCB *next
	)
{
	TCB *prev = current;
	current = next;

	switch_from_to(prev, current);
}


void do_schedule
(void)
{
	switch_to(NEXT(current));
}
