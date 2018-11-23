#pragma once


#include <page.h> // PG_*


extern
unsigned long pgd_map
	( unsigned long *pgd
	, unsigned long va
	, unsigned long pte
	);

extern
void pgd_unmap_free_psm_pages
	( unsigned long *pgd
	, unsigned long beg_addr
	, unsigned long end_addr
	, int invalidate
	);


// TODO: consider #define map with an assert(pte & PGMASK..wiaht)?
#define pgd_unmap(pgd, va)  pgd_map(pgd, va, 0)
#define pgd_getmap(pgd, va) pgd_map(pgd, va, PGMASK)


#include <mmu.h>

static inline
unsigned long map
	( unsigned long va
	, unsigned long pte
	)
{
	unsigned long *pgd = (unsigned long *) mmu_get_pd();
	unsigned long old_pte = pgd_map(pgd, va, pte);
	if (pte != PGMASK)
		mmu_invalidate(pte & PGMASK);
	return old_pte;
}

static inline
void unmap_free_psm_pages
	( unsigned long beg_addr
	, unsigned long end_addr
	)
{
	unsigned long *pgd = (unsigned long *) mmu_get_pd();
	pgd_unmap_free_psm_pages(pgd, beg_addr, end_addr, 1);
}

#define unmap(pa)  map(pa, 0)
#define getmap(pa) map(pa, PGMASK)
