#include <unistd.h>
#include <stdio.h>
#include <stdarg.h>
#include <crt.h>

extern int main(const char *arg, size_t arglen);
extern void init_heap(void); // malloc.c

FILE *stdin, *stdout, *stderr;

static const char *arg;
static size_t arglen;

const char *crt_get_arg(int *plen)
{
	if (plen)
		*plen = arglen;
	return arg;
}

void __attribute__((weak, noreturn)) __crt_start(size_t _arglen, ...)
{
	va_list ap;
	va_start(ap, _arglen);
	arg = (char *) ap;
	arglen = _arglen;
	va_end(ap);

	init_heap();

	stdin = fdopen(0, "r");
	stdout = fdopen(1, "w");
	stderr = stdout;

	int res = main(arg, arglen);

	fdclose(stdin);
	fdclose(stdout);

	exit(res);
}
