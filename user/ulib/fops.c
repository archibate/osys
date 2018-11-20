#include <stdio.h>
#include <unistd.h>

int fflush(FILE *f)
{
	return flush(f->f_fd);
}

int fputc(FILE *f, int c)
{
	return putch(f->f_fd, c);
}

int fgetc(FILE *f)
{
	return getch(f->f_fd);
}

int fread(void *p, size_t size, size_t count, FILE *f)
{
	return read(f->f_fd, p, size * count);
}

int fwrite(const void *p, size_t size, size_t count, FILE *f)
{
	return write(f->f_fd, p, size * count);
}
