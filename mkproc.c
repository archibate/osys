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


PCB *create_process_ex
	( const char *name
	, int (*proc)()
	, void *arg
	)
{
	PCB *pcb = kmalloc(STACK_SIZE);
	bzero(pcb, sizeof(PCB));
	pcb->name = name;

	unsigned long *sp = (unsigned long*)((char*)pcb + STACK_SIZE);
	*--sp = (unsigned long) arg;
	*--sp = (unsigned long) __process_exit;

	pcb->sp = (KS_REGS *) sp;
	bzero(--pcb->sp, sizeof(KS_REGS));
	pcb->sp->eflags = 0x002;
	pcb->sp->pc = (unsigned long) proc;

	unsigned long *pgd = (unsigned long *) alloc_ppage();
	memset(pgd, 0, PGSIZE);
	setup_pgd(pcb->pgd = pgd);

	for (int va = IFRAME_BOTT; va < IFRAME_TOP; va += PGSIZE) {
		unsigned long pgaddr = alloc_ppage();
		memset((void*)pgaddr, 0, PGSIZE);
		pgd_map(pgd, va, pgaddr | PG_PSM | PG_P | PG_W);
	}

	UPCB_OF(pcb)->brk = USER_STACK_BEG;

	return pcb;
}
