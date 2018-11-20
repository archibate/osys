#include <stdio.h>
#include <unistd.h>

int fflush(FILE *f)
{
	return flush(f->f_fd);
}

int fputc(int c, FILE *f)
{
	int res = putch(f->f_fd, c);
	if (c == '\n')
		fflush(f); // TODO: use wr_check_refill_buf??
	return res;
}

static
void check_refill_buf(FILE *f, void (*will_wait_cb)(void))
{
	if (f->f_bpos >= f->f_bsize) {
		will_wait_cb();
		f->f_bsize = read(f->f_fd, f->f_buf, BUFSIZ);
		f->f_bpos = 0;
	}
}

int fgetc_ex(FILE *f, void (*will_wait_cb)(void))
{
	check_refill_buf(f, will_wait_cb);
	return f->f_buf[f->f_bpos++];
}

int fread(void *p, size_t size, size_t count, FILE *f)
{
	return read(f->f_fd, p, size * count);
}

int fwrite(const void *p, size_t size, size_t count, FILE *f)
{
	return write(f->f_fd, p, size * count);
}
