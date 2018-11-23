// vim: st=4 sts=4 sw=4
#include <stbrk.h>
#include <sched.h>
#include <panic.h>
//#include <print.h>
#include <map.h>
#include <psm.h>

int setbrk(void *p)
{
	unsigned long adr = (unsigned long) p;
	unsigned long cur = curr_upcb.brk;

	//printf("setbrk: %p -> %p\n", cur, adr);

	assert(!(cur & PGATTR));
	if (cur < adr)
	{
		for (; cur < adr; cur += PGSIZE)
		    map(cur, alloc_ppage() | PG_PSM | PG_P | PG_W | PG_U);
	}
	else if (cur - PGSIZE > adr)
	{
		for (; cur - PGSIZE < adr; cur += PGSIZE)
		    free_ppage(PGMASK & unmap(cur));
	}

	curr_upcb.brk = cur;

	return 0;
}
