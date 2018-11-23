#include <tss.h>
#include <pid.h>
#include <memlay.h>
#include <print.h>
#include <sched.h>
#include <umove.h>
#include <uregs.h>
#include <kmalloc.h>
#include <mkproc.h>
#include <mkthrd.h>
#include <umemlay.h>
#include <psm.h>
#include <memory.h>
#include <map.h>
#include <mmu.h>


static
void l2_copy_pages
	( unsigned long *to_pgd
	, unsigned long beg
	, unsigned long end
	)
{
	unsigned long *my_pgd = (unsigned long *) mmu_get_pd();
	unsigned long pde, *to_pt, *my_pt;
	unsigned long pte, *to_pg, *my_pg;

	beg >>= (PGSHIFT + PTSHIFT);
	end >>= PGSHIFT;
	end += PTSIZE - 1;
	end >>= PTSHIFT;

	for (int i = beg; i < end; i++) {
		pde = my_pgd[i];
		if (pde & PG_P) {
			my_pt = (unsigned long *) (pde & PGMASK);
			to_pt = (unsigned long *) alloc_ppage();

			pde = (unsigned long) to_pt | (pde & PTATTR);

			for (int j = 0; j < PTSIZE; j++) {
				pte = my_pt[j];
				if (pte & PG_P) {
					my_pg = (unsigned long *) (pte & PGMASK);
					to_pg = (unsigned long *) alloc_ppage();

					memcpy(to_pg, my_pg, PGSIZE);

					pte = (unsigned long) to_pg | (pte & PTATTR);
				} else
					pte = 0;
				to_pt[j] = pte;
			}
		} else
			pde = 0;
		to_pgd[i] = pde;
	}
}

static
void l1_copy_pages
	( unsigned long *to_pgd
	, unsigned long beg
	, unsigned long end
	)
{
	unsigned long *my_pgd = (unsigned long *) mmu_get_pd();
	unsigned long pde, *to_pt, *my_pt;

	beg >>= (PGSHIFT + PTSHIFT);
	end >>= PGSHIFT;
	end += PTSIZE - 1;
	end >>= PTSHIFT;

	for (int i = beg; i < end; i++) {
		pde = my_pgd[i];
		if (pde & PG_P) {
			my_pt = (unsigned long *) (pde & PGMASK);
			to_pt = (unsigned long *) alloc_ppage();

			pde = (unsigned long) to_pt | (pde & PTATTR);

			memcpy(to_pt, my_pt, PGSIZE);
		} else 
			pde = 0;

		to_pgd[i] = pde;
	}
}


static
void user_space_copy_to(unsigned long *to_pgd)
{
	//printf("user_space_copy_to\n");
	l2_copy_pages(to_pgd, USER_CODE_BEG,   USER_CODE_END);
	l1_copy_pages(to_pgd, USER_MMAP_BEG,   USER_MMAP_END);
	l2_copy_pages(to_pgd, USER_STACK_BEG, USER_STACK_END);
	memcpy(UPCB_OF(to_pgd),   &curr_upcb,   sizeof(UPCB));
}


static
int __attribute__((noreturn)) forked_entry_fc(IF_REGS *uregs)
{
	IF_REGS mregs;
	memcpy(&mregs, uregs, sizeof(IF_REGS));
	kfree(uregs);

	//printf("forked_entry_fc\n");

	mregs.ax = 0;
	move_to_user(&mregs);
}


int sys_fork(void)
{
	IF_REGS *if_regs = (IF_REGS*)IFRAME_TOP - 1;
	IF_REGS *uregs = kmalloc_for(IF_REGS);
	memcpy(uregs, if_regs, sizeof(IF_REGS));
	PCB *pcb = create_process_ex(current->pcb->name, forked_entry_fc, uregs);
	user_space_copy_to(pcb->pgd);
	int pid = new_proc(create_thread(pcb));
	return pid;
}
