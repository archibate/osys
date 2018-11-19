#include <kern/sysapi.h>
#include <stdio.h>

extern int main(void);
extern void init_heap(void);

FILE *stdin, *stdout, *stderr;

void __attribute__((weak, noreturn)) crt_start(void)
{
	init_heap();

	stdin = fopen("/dev/kbd0", "r");
	stdout = fopen("/dev/mon0", "w");
	stderr = fopen("/dev/mon0", "w");

	int res = main();

	fclose(stdin);
	fclose(stdout);
	fclose(stderr);

	exit(res);
}
