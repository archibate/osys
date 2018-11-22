#include <pexit.h>
#include <exec.h>
#include <panic.h>

int __attribute__((noreturn)) sys_undefined(void)
{
	panic("undefined system call");
}

#define verify_ptr(p) if ((unsigned long) (p) < DMA_END) return -E_SEG_FL


int sys_stexecv1(const char *name, const char *arg)
{
	return stexecv1(name, arg);
}

void __attribute__((noreturn)) sys_exit(int status)
{
	process_exit(status);
}
