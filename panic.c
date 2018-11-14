#include <panic.h>
#include <print.h>
#include <stdarg.h>

static
void __attribute__((noreturn)) cpu_halt(void)
{
	asm volatile ("cli\nhlt\n");
	__builtin_unreachable();
}


static
void __attribute__((noreturn)) halt(void)
{
	printf("cpu dump:\n");

	unsigned long cr2;
	asm volatile ("movl %%cr2, %%eax\n" : "=a" (cr2));
	printf("cr2=%p\n", cr2);

	cpu_halt();
}


void __attribute__((noreturn)) panic
		( const char *fmt
		, ...
		)
{
	va_list ap;
	va_start(ap, fmt);
	printf(C_EMPH C_RED "PANIC: ");
	vprintf(fmt, ap);
	printf(C_NORM "\n");
	va_end(ap);

	halt();
}
