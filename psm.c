#include <psm.h>
#include <panic.h>
#include <memlay.h>
#include <page.h> // PGSIZE


void init_psm
(void)
{
	unsigned long addr;
	for	( addr = PSM_BEG
		; addr < PSM_END
		; addr += PGSIZE
		) free_ppage(addr);
}


#define PSM_STACK_SIZE (((PSM_END - PSM_BEG) / PGSIZE) + 8)

static unsigned long psm_stack[PSM_STACK_SIZE], *psm_sp = psm_stack+4;


void free_ppage
(unsigned long addr)
{
	assert(psm_sp < psm_stack + PSM_STACK_SIZE);
	*psm_sp++ = addr;
}

unsigned long alloc_ppage
(void)
{
	assert(psm_sp - 1 >= psm_stack);
	return *--psm_sp;
}
