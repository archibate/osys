#include <pcb.h>
#include <mkproc.h> // impelementation
#include <memory.h>
#include <kmalloc.h>
#include <panic.h>

#define STACK_SIZE 4096


void __attribute__((noreturn)) process_exit(int val)
{
	panic("process exited with %d\n", val);
}

static
void __process_exit(void)
{
	register int val asm ("eax");
	process_exit(val);
}


PCB *create_process
	( int (*proc)()
	, void *arg
	)
{
	PCB *pcb = kmalloc(STACK_SIZE);

	unsigned long *sp = (unsigned long*)((char*)pcb + STACK_SIZE);
	*--sp = (unsigned long) arg;
	*--sp = (unsigned long) __process_exit;

	pcb->sp = (KS_REGS *) sp;
	bzero(--pcb->sp, sizeof(KS_REGS));
	pcb->sp->pc = (unsigned long) proc;

	return pcb;
}
