#include <pcb.h>
#include <mkproc.h> // impelementation
#include <memory.h>
#include <kmalloc.h>
#include <panic.h>
#include <texit.h>

#define STACK_SIZE 4096

static
void __attribute__((noreturn)) __process_exit(void)
{
	register int val asm ("eax");
	thread_exit(val);
}


PCB *create_process
	( int (*proc)()
	, void *arg
	)
{
	PCB *pcb = kmalloc(STACK_SIZE);
	bzero(pcb, sizeof(PCB));

	unsigned long *sp = (unsigned long*)((char*)pcb + STACK_SIZE);
	*--sp = (unsigned long) arg;
	*--sp = (unsigned long) __process_exit;

	pcb->sp = (KS_REGS *) sp;
	bzero(--pcb->sp, sizeof(KS_REGS));
	pcb->sp->eflags = 0x202;
	pcb->sp->pc = (unsigned long) proc;

	return pcb;
}
