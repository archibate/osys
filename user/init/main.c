#include <stdio.h>
#include <unistd.h>

int main(const char *arg)
{
	if (!fork()) {
		open("/dev/kbd0", OPEN_RD);
		open("/dev/mon0", OPEN_WR);
		open("/dev/mon0", OPEN_WR);
		exit(execap("/bin/xfont.exf", ""));
	}


	if (!fork()) {
		open("/dev/kbd0", OPEN_RD);
		open("/dev/vmon0", OPEN_WR);
		open("/dev/vmon0", OPEN_WR);
		exit(execap("/bin/shell.exf", ""));
	}

	return 0;
}
