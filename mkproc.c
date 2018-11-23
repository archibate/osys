#include <pcb.h>
#include <mkproc.h> // impelementation
#include <page.h>
#include <memory.h>
#include <umemlay.h>
#include <memlay.h>
#include <map.h>
#include <kmalloc.h>
#include <schpcb.h>
#include <panic.h>
#include <pexit.h>
#include <psm.h>
#include <upcb.h>
#include <print.h>

#define STACK_SIZE 8192

static
void __attribute__((noreturn)) __process_exit(void)
{
	register int val asm ("eax");
	process_exit(val);
}


TCB *create_process_ex
	( const char *name
	, int (*proc)()
	, void *arg
	)
{
	TCB *tcb = kmalloc(STACK_SIZE);
	bzero(tcb, sizeof(PCB));
	tcb->pcb.name = name;

	unsigned long *sp = (unsigned long*)((char*)tcb + STACK_SIZE);
	*--sp = (unsigned long) arg;
	*--sp = (unsigned long) __process_exit;

	tcb->pcb.sp = (KS_REGS *) sp;
	bzero(--tcb->pcb.sp, sizeof(KS_REGS));
	tcb->pcb.sp->eflags = 0x002;
	tcb->pcb.sp->pc = (unsigned long) proc;

	unsigned long *pgd = (unsigned long *) alloc_ppage();
	memset(pgd, 0, PGSIZE);
	setup_pgd(pgd);

	for (int va = IFRAME_BOTT; va < IFRAME_TOP; va += PGSIZE) {
		unsigned long pgaddr = alloc_ppage();
		memset((void*)pgaddr, 0, PGSIZE);
		pgd_map(pgd, va, pgaddr | PG_PSM | PG_P | PG_W);
	}

	UPCB_OF(pgd)->brk = USER_STACK_BEG;

	tcb->pcb.pgd = pgd;

	return tcb;
}
