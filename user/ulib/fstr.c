#include <stdio.h>
#include <string.h>

int fputs(const char *s, FILE *f)
{
	return fwrite(s, strlen(s), 1, f);
}
