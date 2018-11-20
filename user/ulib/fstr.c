#include <stdio.h>
#include <string.h>
#include <unistd.h>

int fputs(const char *s, FILE *f)
{
	return fwrite(s, strlen(s), 1, f);
}

int fgetline(char *s, size_t size, FILE *f)
{
	size = read(f->f_fd, s, size);
	s[size] = 0;
	return size;
}
