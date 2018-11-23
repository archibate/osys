#include <pcb.h>
#include <mkproc.h> // impelementation
#include <page.h>
#include <memory.h>
#include <umemlay.h>
#include <kmalloc.h>
#include <schpcb.h>
#include <panic.h>
#include <pexit.h>
#include <psm.h>
//#include <print.h>

#define STACK_SIZE 8192

static
void __attribute__((noreturn)) __process_exit(void)
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
	bzero(pcb, sizeof(PCB));

	unsigned long *sp = (unsigned long*)((char*)pcb + STACK_SIZE);
	*--sp = (unsigned long) arg;
	*--sp = (unsigned long) __process_exit;

	pcb->sp = (KS_REGS *) sp;
	bzero(--pcb->sp, sizeof(KS_REGS));
	pcb->sp->eflags = 0x202;
	pcb->sp->pc = (unsigned long) proc;

	//printf("create: %p:%p\n", pcb, pcb->sp);

	unsigned long *pgd = (unsigned long *) alloc_ppage();
	memset(pgd, 0, PGSIZE);
	setup_pgd(pcb->pgd = pgd);

	pcb->brk = USER_STACK_BEG;

	return pcb;
}
