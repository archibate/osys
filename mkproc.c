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
#include <uregs.h>
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
	//printf("create_process_ex(%s,%p,%p)\n", name, proc, arg);

	TCB *tcb = kmalloc(sizeof(TCB));
	bzero(tcb, sizeof(TCB));
	tcb->name = name;

	unsigned long *pgd = (unsigned long *) alloc_ppage();
	memset(pgd, 0, PGSIZE);
	setup_pgd(pgd);

	for (int va = IFRAME_BOTT; va < IFRAME_TOP; va += PGSIZE) {
		unsigned long pgaddr = alloc_ppage();
		memset((void*)pgaddr, 0, PGSIZE);
		pgd_map(pgd, va, pgaddr | PG_PSM | PG_P | PG_W);
	}

	UPCB_OF(pgd)->brk = USER_STACK_BEG;


	unsigned long *sp = (unsigned long *) IFRM_OF(pgd);
	*--sp = (unsigned long) arg;
	*--sp = (unsigned long) __process_exit;

	tcb->sp = (KS_REGS *) sp;
	bzero(--tcb->sp, sizeof(KS_REGS));
	tcb->sp->eflags = 0x002;
	tcb->sp->pc = (unsigned long) proc;

	tcb->pgd = pgd;

	return tcb;
}
