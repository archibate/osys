#pragma once


#include <page.h> // PG_*


extern
unsigned long __map
	( unsigned long *pgd
	, unsigned long va
	, unsigned long pte
	);


// TODO: consider #define map with an assert(pte & PGMASK..wiaht)?
#define __unmap(pgd, va)  __map(pgd, va, 0)
#define __getmap(pgd, va) __map(pgd, va, PGMASK)


#include <mmu.h>

static inline
unsigned long map
	( unsigned long va
	, unsigned long pte
	)
{
	unsigned long *pgd = (unsigned long *) mmu_get_pd();
	unsigned long old_pte = __map(pgd, va, pte);
	if (pte != PGMASK)
		mmu_invalidate(pte & PGMASK);
	return old_pte;
}

#define unmap(pa)  map(pa, 0)
#define getmap(pa) map(pa, PGMASK)
