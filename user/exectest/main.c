#include <unistd.h>
#include <stdio.h>

int main(void)
{
	puts("exectest Here!\n");
	return execap("/bin/true.exf", "Hello, World!");
}
