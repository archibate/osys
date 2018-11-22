#include <unistd.h>
#include <stdio.h>
#include <stdarg.h>
#include <crt.h>

extern int main(void);
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

	stdin = fopen("/dev/kbd0", "r");
#ifndef NO_VMON
	stdout = fopen("/dev/vmon0", "w");
	stderr = fopen("/dev/vmon0", "w");
#else
	stdout = fopen("/dev/mon0", "w");
	stderr = fopen("/dev/mon0", "w");
#endif

	int res = main();

	fclose(stdin);
	fclose(stdout);
	fclose(stderr);

	exit(res);
}
