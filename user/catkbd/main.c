#include <kern/sysapi.h>
#include <fsdefs.h>
#include <stddef.h>

void __attribute__((noreturn)) error(int res, const char *msg);

int main(void)
{
	int i, j, c;

	i = open("/dev/kbd0", OPEN_RD);
	if (i < 0) { error(i, "open(/dev/kbd0)"); }

	j = open("/dev/mon0", OPEN_WR);
	if (j < 0) { error(j, "open(/dev/mon0)"); }

	if (i == j) asm volatile ("cli; hlt");

	while (EOF != (c = getch(i)))
		putch(j, c);

	close(i);
	close(j);
	return 0;
}

void __attribute__((noreturn)) error(int res, const char *msg)
{
	exit(res);
}
