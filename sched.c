#include <sched.h>
#include <schpcb.h>
#include <memory.h>
#include <panic.h>


TCB *current;


#ifndef TESTING
#define tprintf(...)
#else
#include <print.h>
#define tprintf(...) printf(__VA_ARGS__)
#include <mkthrd.h>
#include <mkproc.h>

static
int fuck_func
(void)
{
	for (;;) asm volatile ("hlt");
	return 0;
}

static
void __attribute__((unused)) print_sched_status
(void);

void test_sched
(void)
{
	PCB *pcb = create_process(fuck_func, 0);
	TCB *tcb = create_thread(pcb);
	printf("created: t%p p%p sp=%p\n", tcb, pcb, pcb->sp);
	print_sched_status();
}
#endif


void print_sched_status
(void)
{
	printf("sched status:\n");
	TCB *curr = current;
	do {
		printf("t%p p%p: sp=%p\n", curr, curr->pcb, curr->pcb->sp);
		curr = NEXT(curr);
	} while (curr != current);
}


void init_sched
(void)
{
	static char kstk0[1024];
	static TCB tcb0;
	static PCB pcb0;

	pcb0.sp = (KS_REGS *) (kstk0 + sizeof(kstk0) - sizeof(KS_REGS));
	bzero(pcb0.sp, sizeof(KS_REGS));

	tcb0.list = SELF_LOOP(LI(&tcb0));
	tcb0.pcb = &pcb0;
	current = &tcb0;
}


void add_task
	( TCB *tcb
	)
{
	list_insert_after(LI(current), LI(tcb));
}


static
void switch_to
	( TCB *next
	)
{
	TCB *prev = current;
	current = next;

	switch_from_to(prev->pcb, current->pcb);
}


void do_schedule
(void)
{
	switch_to(NEXT(current));
}
