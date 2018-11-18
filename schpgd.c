#include <umemlay.h>
#include <memory.h>
#include <page.h>
#include <mmu.h>
//#include <print.h>

void switch_pgd(unsigned long *pgd)
{
	unsigned long *prev_pgd = (unsigned long *) mmu_get_pd();
	//printf("%p->%p\n", prev_pgd, pgd);
	if (pgd != prev_pgd) {
		memcpy(pgd, prev_pgd, PGSIZE);//USER_BEG >> (PGSHIFT + PTSHIFT));
		mmu_set_pd(pgd);
	}
	//printf("done\n");
}
