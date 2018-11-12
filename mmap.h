#pragma once


#include <page.h> // PG_*


extern
unsigned long __mmap
	( unsigned long *pgd
	, unsigned long pa
	, unsigned long pte
	);


// TODO: consider #define mmap with an assert(pte & PGMASK..wiaht)?
#define __unmap(pgd, pa)  __mmap(pgd, pa, 0)
#define __getmap(pgd, pa) __mmap(pgd, pa, PGMASK)


#include <mmu.h>

static inline
unsigned long mmap
	( unsigned long pa
	, unsigned long pte
	)
{
	unsigned long *pgd = (unsigned long *) mmu_get_pd();
	unsigned long old_pte = __mmap(pgd, pa, pte);
	if (pte != PGMASK)
		mmu_invalidate(pte & PGMASK);
	return old_pte;
}

#define unmap(pa)  mmap(pa, 0)
#define getmap(pa) mmap(pa, PGMASK)
