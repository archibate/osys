#include <umemlay.h>
#include <memory.h>
#include <page.h>
#include <mmu.h>

void switch_pgd(unsigned long *pgd)
{
	unsigned long *prev_pgd = (unsigned long *) mmu_get_pd();

	if (pgd != prev_pgd) {
		memcpy(pgd, prev_pgd, USER_BEG >> (PGSHIFT + PTSHIFT));
		mmu_set_pd(pgd);
	}
}
