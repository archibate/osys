#include <vfs.h>
#include <print.h>
#include <kmalloc.h>
#include <umemlay.h>
#include <uload.h>
#include <umove.h>
#include <sched.h>
#include <schpcb.h>
#include <map.h>
#include <psm.h>
#include <mmu.h>
#include <memory.h>

void __attribute__((noreturn)) transfer_to_user(void)
{
	static IF_REGS uregs = {
		.pc = USER_TEXT_BEG,
		.sp = USER_STACK_END,
		.eflags = 0x202,
		.cs = 0x001b,
		.ss = 0x0023,
		.ds = 0x0023,
		.es = 0x0023,
	};
	move_to_user(&uregs);
}

int load_user_program(const char *name)
{
	FILE *f = kmalloc_for(FILE);
	int res = open(f, name, OPEN_RD);
	if (res)
		goto out_free;

	void *addr = (void *) USER_TEXT_BEG;

	unsigned long *pgd = (unsigned long *) alloc_ppage();
	switch_pgd(current->pcb->pgd = pgd);

	map(0x20000000, 0x7000 | PG_P | PG_U);

	res = mmap(f, addr, f->f_size, MMAP_USR);
	if (res < 0)
		goto out_close;
	else
		res = 0;

	unsigned int pages = (name[5] == 'g' ? 0xd00 : 0x5);
	unsigned long stkbtm = USER_STACK_END;
	for (int i = 0; i < pages; i++)
		map(stkbtm -= PGSIZE, alloc_ppage() | PG_P | PG_W | PG_U | PG_PSM);

out_close:
	close(f);
out_free:
	kfree(f);
	return res;
}

void on_user_exit(void)
{
	unmap_free_psm_non_global_pages();
}
