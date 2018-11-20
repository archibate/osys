#include <pexit.h>
#include <exec.h>
#include <panic.h>

int __attribute__((noreturn)) sys_undefined(void)
{
	panic("undefined system call");
}

#define verify_ptr(p) if ((unsigned long) (p) < DMA_END) return -E_SEG_FL


int sys_stexec(const char *name)
{
	return stexec(name);
}

void __attribute__((noreturn)) sys_exit(int status)
{
	process_exit(status);
}
