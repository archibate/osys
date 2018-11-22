#include <stdio.h>
#include <crt.h>

int main(void)
{
	int arglen;
	const char *arg = crt_get_arg(&arglen);
	printf("%.*s\n", arglen, arg);
	return 0;
}
