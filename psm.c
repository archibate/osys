#include "psm.h"
#include "memlay.h"
#include "page.h" // PGSIZE


void init_psm
(void)
{
	unsigned long addr;
	for	( addr = PSM_BEG
		; addr < PSM_END
		; addr += PGSIZE
		) free_ppage(addr);
}


#define PSM_STACK_SIZE ((PSM_END - PSM_BEG) / PGSIZE)

static unsigned long psm_stack[PSM_STACK_SIZE+2], *psm_sp = psm_stack+1;


void free_ppage
(unsigned long addr)
{
	*psm_sp++ = addr;
}

unsigned long alloc_ppage
(void)
{
	return *--psm_sp;
}
