#include <stbrk.h>
#include <page.h>
#include <umemlay.h>
#include <errno.h>

#define verify_addr(p) if ((p)<USER_STACK_BEG || (p)>USER_STACK_END) return -E_SEG_FL

int sys_setbrk(void *p)
{
	verify_addr((unsigned long) p);

	return setbrk(p);
}
