#pragma once


#include "page.h" // PG_*


extern
unsigned long mmap
	( unsigned long *pgd
	, unsigned long pa
	, unsigned long pte
	);


// TODO: consider #define mmap with an assert(pte & PGMASK..wiaht)?
#define unmap(pgd, pa)  mmap(pgd, pa, 0)
#define getmap(pgd, pa) mmap(pgd, pa, PGMASK)
