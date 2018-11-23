#include <stdio.h>
#include <unistd.h>

int main(void)
{
	int pid = fork();
	if (!pid) {
		printf("child process!\n");
		exit(execap("/bin/die.exf", ""));
	}
	unsigned long *upcb = (unsigned long *) 0x80000000;
	map_upcb(pid, upcb, MMAP_WR);
	printf("upcb.brk = %p!\n", upcb[0]);
	waiton(pid, EVE_EXIT);
	return 0;
}
