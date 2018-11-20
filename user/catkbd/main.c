#include <kern/sysapi.h>
#include <fsdefs.h>
#include <stddef.h>

int main(void)
{
	int i, j, c;

	i = open("/dev/kbd0", OPEN_RD);
	if (i < 0) exit(i);

	j = open("/dev/vmon0", OPEN_WR);
	if (j < 0) exit(j);

	while (EOF != (c = getch(i))) {
		putch(j, c);
		fsync(j);
	}

	close(i);
	close(j);
	return 0;
}
