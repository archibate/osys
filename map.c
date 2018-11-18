#include <map.h>
#include <psm.h>
#include <panic.h>
#include <memory.h>


unsigned long __map
	( unsigned long *pgd
	, unsigned long va
	, unsigned long pte
	)
{
	unsigned ptsel, pdsel;
	unsigned long pde, *pt, old_pte;

	va >>= PGSHIFT;
	ptsel = va & PTATTR;
	pdsel = va >> PTSHIFT;

	pde = pgd[pdsel];
	if (!(pde & PG_P)) {
		if (!(pte & PG_P)) // unmap or get_mapping
			return 0;

		pde = alloc_ppage();
		bzero((void *) pde, PGSIZE);
		pde |= PG_P | PG_W | PG_U;
		pgd[pdsel] = pde;
	}

	pt = (unsigned long *) (pde & PGMASK);
	old_pte = pt[ptsel];

	if (/*(*/pte/* & (PGMASK | PG_P))*/ != PGMASK)
		pt[ptsel] = pte;

	return old_pte;
}
