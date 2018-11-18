#include <uregs.h>
#include <panic.h>
#include <print.h>

void __attribute__((noreturn)) unhandled_exception(const char *info)
{
	/*void *****bp;
	asm volatile ("mov %%ebp, %0" : "=r" (bp));
	IF_REGS *regs = (IF_REGS *) (bp[0] + 2);

	printf("ss:esp=%04X:%p\n", regs->ss, regs->sp);
	printf("cs:eip=%04X:%p\n", regs->ss, regs->pc);
	printf("err=%08X\tefl=%08X\n", regs->exp_errcd, regs->eflags);
	printf("eax=%p\tebx=%p\n", regs->ax, regs->bx);
	printf("ecx=%p\tedx=%p\n", regs->cx, regs->dx);
	printf("esi=%p\tedi=%p\n", regs->si, regs->di);
	printf("ebp=%p\tesp=%p\n", regs->bp, regs->unused_sp);*/

	panic(info);
}
