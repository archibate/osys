#include <stdio.h>
#include <unistd.h>
#include <assert.h>
#include <string.h>
#include <memory.h>

int fflush(FILE *f)
{
	if (f->f_oattr & OPEN_WR)
		file_wr_flush(f);
	if (f->f_oattr & OPEN_RD)
		file_rd_flush(f);
	return 0;
}

const char *fgetrdbuf(FILE *f)
{
	if (f->f_bpos >= f->f_bsize)
		file_rd_flush(f);
	f->f_buf[f->f_bsize] = 0;
	const char *s = f->f_buf + f->f_bpos;
	f->f_bpos = f->f_bsize;
	return s;
}

int fgets(char *s, size_t size, FILE *f)
{
	unsigned i, n = size - 1;
	const char *buf;

	assert(size > 1);

	do {
		buf = fgetrdbuf(f);
		if (!(i = strfind(buf, '\n') + 1))
			i = strlen(buf);

		if (i > n) i = n;
		memcpy(s, buf, i);
		s += i, n -= i;

	} while (n && buf[i - 1] != '\n');

	*s++ = 0;

	return size - n;
}

int fputs(const char *s, FILE *f)
{
	int need_fl = 0;

	while (*s) {
		__fputc(*s++, f);
		need_fl |= file_need_wr_flush(f);
	}
	if (need_fl)
		file_wr_flush(f);

	return need_fl;
}

int file_need_wr_flush(FILE *f)
{
	return f->f_buf[f->f_bpos - 1] == '\n' || f->f_bpos >= BUFSIZ;
}

void file_wr_flush(FILE *f)
{
	write(f->f_fd, f->f_buf, f->f_bpos);
	f->f_bpos = 0;
}

void file_rd_flush(FILE *f)
{
	f->f_bsize = read(f->f_fd, f->f_buf, BUFSIZ);
	f->f_bpos = 0;
}

int fread(void *p, size_t size, size_t count, FILE *f)
{
	return read(f->f_fd, p, size * count);
}

int fwrite(const void *p, size_t size, size_t count, FILE *f)
{
	return write(f->f_fd, p, size * count);
}
