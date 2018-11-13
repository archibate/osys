#include <schpcb.h>
#include <print.h>


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
	__switch_from_to(&prev->sp, &next->sp);
}
