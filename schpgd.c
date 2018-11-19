#include <umemlay.h>
#include <memory.h>
#include <page.h>
#include <mmu.h>
#include <print.h>

int setup_pgd(unsigned long *pgd)
{
	unsigned long *old_pgd = (unsigned long *) mmu_get_pd();

	if (pgd == old_pgd)
		return 0;

	memcpy(pgd, old_pgd, USER_BEG >> (PGSHIFT + PTSHIFT));
	return 1;
}

void switch_pgd(unsigned long *pgd)
{
	if (setup_pgd(pgd))
		mmu_set_pd(pgd);
}
