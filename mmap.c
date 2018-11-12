#include "mmap.h"
#include "psm.h"
#include "panic.h"


unsigned long mmap
	( unsigned long *pgd
	, unsigned long pa
	, unsigned long pte
	)
{
	unsigned ptsel, pdsel;
	unsigned long pde, *pt, old_pte;

	pa >>= PGSHIFT;
	ptsel = pa & PTMASK;
	pdsel = pa >> PTSHIFT;

	pde = pgd[pdsel];
	if (!(pde & PG_P)) {
		if (pte & PG_P) {
			pde = alloc_page();
			pde |= PG_P | PG_W;
			pgd[pdsel] = pde;
		} else {
			return 0;
		}
	}

	pt[ptsel] = 

	pt = (unsigned long *) (pde & PGMASK);
	old_pte = pt[ptsel];

	if (pte & (PGMASK | PG_P) == PGMASK)
		pt[ptsel] = pte;

	return old_pte;
}
