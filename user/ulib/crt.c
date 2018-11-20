#include <kern/sysapi.h>
#include <stdio.h>

extern int main(void);
extern void init_heap(void);

FILE *stdin, *stdout, *stderr;

void __attribute__((weak, noreturn)) crt_start(void)
{
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
