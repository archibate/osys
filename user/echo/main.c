#include <stdio.h>

int main(const char *arg, size_t arglen)
{
	printf("%.*s\n", arglen, arg);
	return 0;
}
