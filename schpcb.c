#include <schpcb.h>


extern
void __attribute__((fastcall)) __switch_from_to
	( KS_REGS **prev
	, KS_REGS **next
	);

void switch_from_to
	( PCB *prev
	, PCB *next
	)
{
	switch_pgd(next->pgd);

	//printf("%p:%p -> %p:%p\n", prev, prev->sp, next, next->sp);
	if (prev != next)
		__switch_from_to(&prev->sp, &next->sp);
}
