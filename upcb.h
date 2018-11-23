#pragma once

#include <pcbdefs.h>
#include <struct.h>
#include <event.h>

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
#define UPCB_OF(pgd) ((UPCB *) (pgd_getmap((pgd), IFRAME_BOTT) & PGMASK))
