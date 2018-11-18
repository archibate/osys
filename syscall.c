#include <uregs.h>
#include <panic.h>

void system_call(PT_REGS *regs)
{
	panic("SYSTEM_CALL");
}
