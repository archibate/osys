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
	printf("switch from %p to %p\n", prev, next);
	__switch_from_to(&prev->sp, &next->sp);
}
