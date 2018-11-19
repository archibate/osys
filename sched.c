#include <sched.h>
#include <schpcb.h>
#include <memory.h>
#include <panic.h>


TCB *current;


#ifndef TESTING /////
#define tprintf(...)
#else
#include <print.h>
#define tprintf(...) printf(__VA_ARGS__)
#include <mkthrd.h>
#include <mkproc.h>
#include <event.h>

static
EVENT eve_cam;

static
int suck_func
(void)
{
	printf("suck_func in\n");
	wait_on(&eve_cam);
	printf("suck_func out\n");
	for (;;) asm volatile ("hlt");
	return 0;
}

static
int dick_func
(void)
{
	printf("dick_func in\n");
	trig_up(&eve_cam);
	printf("dick_func out\n");
	for (;;) asm volatile ("hlt");
	return 0;
}

static
void __attribute__((unused)) print_sched_status
(void);

void test_sched
(void)
{
	create_thread(create_process(dick_func, 0));
	create_thread(create_process(suck_func, 0));
	print_sched_status();
}
#endif /////


#ifdef TESTING
void print_sched_status
(void)
{
	tprintf("sched status:\n");
	TCB *curr = current;
	do {
		tprintf("t%p p%p: sp=%p\n", curr, curr->pcb, curr->pcb->sp);
		curr = curr->next;
	} while (curr != current);
	tprintf("sched status end\n");
}
#endif


void init_sched
(void)
{
	static char kstk0[1024];
	static TCB tcb0;
	static PCB pcb0;

	pcb0.sp = (KS_REGS *) (kstk0 + sizeof(kstk0) - sizeof(KS_REGS));
	bzero(pcb0.sp, sizeof(KS_REGS));

	tcb0.next = tcb0.prev = &tcb0;
	tcb0.pcb = &pcb0;
	current = &tcb0;
}


void add_task
	( TCB *tcb
	)
{
	tcb_insert_after_ch(tcb, &current);
}


void task_run
	( TCB *next
	)
{
	TCB *prev = current;
	current = next;

	switch_from_to(prev->pcb, next->pcb);
}


void do_schedule
(void)
{
	//tprintf("do_schedule()\n");
	assert(current);
	task_run(current->next);
}
