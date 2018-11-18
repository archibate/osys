#include <map.h>
#include <mmu.h>
#include <memlay.h>
#include <psm.h>
#include <panic.h>
#include <memory.h>


unsigned long pgd_map
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

	if (pte & PG_PSM)
		pgd[pdsel] |= PG_PDM;

	pde = pgd[pdsel];

	if (!(pde & PG_P)) {
		if (!(pte & PG_P)) // unmap or get_mapping
			return 0;

		pde = alloc_ppage();
		bzero((void *) pde, PGSIZE);
		pde |= PG_PDM | PG_P | PG_W | PG_U;
		pgd[pdsel] = pde;
	}

	pgd[pdsel] |= pte & PG_PSM;

	pt = (unsigned long *) (pde & PGMASK);
	old_pte = pt[ptsel];

	if (/*(*/pte/* & (PGMASK | PG_P))*/ != PGMASK)
		pt[ptsel] = pte;

	return old_pte;
}


#if 0
void pgd_unmap_free_marked_pages
	( unsigned long *pgd
	, int invalidate
	)
{
	unsigned long pde, pte, *pt;
	int i, j;

	for (i = 0; i < PTSIZE; i++)
	{
		pde = pgd[i];

		if ((pde & (PG_P | PG_G)) != PG_P)
			continue;
		printf("s");

		if (pde & PG_PDM)
		{
			//free_ppage(pde & PGMASK);
			//pgd[i] = 0;
		}
		if (/*1 || (*/pde & PG_PSM/*)*/)
		{
			printf("S");
			pt = (unsigned long *) (pde & PGMASK);

			for (j = 0; j < PTSIZE; j++)
			{
				pte = pt[j];

				if ((pte & (PG_P | PG_PSM | PG_G)) == (PG_P | PG_PSM))
				{
					free_ppage(pte & PGMASK);
					pt[j] = 0;

					if (invalidate)
						mmu_invalidate(pte & PGMASK);
				}
			}
		}
	}
}
#else
void pgd_unmap_free_psm_non_global_pages
	( unsigned long *pgd
	, int invalidate
	) // TODO: 干脆考虑把所有在USER_BEG以后,且物理地址在PSM管辖范围内的页都free了?
{
	unsigned long pde, pte, *pt;
	int i, j;

	for (i = 0; i < PTSIZE; i++)
	{
		pde = pgd[i];

		if ((pde & (PG_P | PG_G)) != PG_P)
			continue;

		if (pde & PG_PDM)
		{
			//free_ppage(pde & PGMASK);
			//pgd[i] = 0;
		}
		if (/*1 || (*/pde & PG_PSM/*)*/)
		{
			pt = (unsigned long *) (pde & PGMASK);

			for (j = 0; j < PTSIZE; j++)
			{
				pte = pt[j];

				if ((pte & (PG_P | PG_PSM | PG_G)) == (PG_P | PG_PSM))
				{
					free_ppage(pte & PGMASK);
					pt[j] = 0;

					if (invalidate)
						mmu_invalidate(pte & PGMASK);
				}
			}
		}
	}
}
#endif
