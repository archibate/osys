#include "schswi.h"
#include "print.h"


void switch_from_to
	( TCB *prev
	, TCB *next
	)
{
	asm volatile (
		"pushl %%ebp\n"
		"pushl %%edi\n"
		"pushl %%esi\n"
		"pushl %%ebx\n"
		"movl %%esp, %%eax\n"
		"movl %%ecx, %%esp\n"
		"popl %%ebx\n"
		"popl %%esi\n"
		"popl %%edi\n"
		"popl %%ebp\n"
		: "=a" (prev->sp)
		: "c"  (next->sp)
		: "cc", "memory"
		);
}
