#pragma once

#include <pcbdefs.h>
#include <struct.h>

STRUCT(UPCB)
{
	unsigned long brk;

	union {
		struct FILE *files[FILES_MAX];
		struct DIR *dirs[FILES_MAX];
	};
};

#include <memlay.h>
#define curr_upcb (*(UPCB *) IFRAME_BOTT)
#define UPCB_OF_PGD(pgd) ((UPCB *) (pgd_getmap((pgd), IFRAME_BOTT) & PGMASK))
