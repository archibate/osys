#include <stdio.h>
#include <unistd.h>

int main(void)
{
	int pid = fork();
	if (!pid) {
		printf("child process!\n");//: upcb[20] = %p!\n", upcb[20]);
		exit(execap("/bin/true.exf", ""));
	}
	int exit_res = waiton(pid, EVE_EXIT);
	printf("child exited with %d\n", exit_res);
	return 0;
}
