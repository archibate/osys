#include <stdio.h>
#include <unistd.h>

void file_wr_flush(FILE *f)
{
	write(f->f_fd, f->f_buf, f->f_bpos);
	f->f_bpos = 0;
}

void file_chk_wr_flush(FILE *f)
{
	if (f->f_buf[f->f_bpos - 1] == '\n' || f->f_bpos >= BUFSIZ)
		file_wr_flush(f);
}

int fputc(int c, FILE *f)
{
	__fputc(c, f);
	file_chk_wr_flush(f);
	return c;
}

void file_rd_flush(FILE *f)
{
	f->f_bsize = read(f->f_fd, f->f_buf, BUFSIZ);
	f->f_bpos = 0;
}

int fgetc_ex(FILE *f, void (*will_wait_cb)(void))
{
	if (f->f_bpos >= f->f_bsize) {
		if (will_wait_cb)
			will_wait_cb();
		file_rd_flush(f);
	}
	return f->f_buf[f->f_bpos++];
}

int fflush(FILE *f)
{
	if (f->f_oattr & OPEN_WR)
		file_wr_flush(f);
	if (f->f_oattr & OPEN_RD)
		file_rd_flush(f);
	return 0;
}

int fread(void *p, size_t size, size_t count, FILE *f)
{
	return read(f->f_fd, p, size * count);
}

int fwrite(const void *p, size_t size, size_t count, FILE *f)
{
	return write(f->f_fd, p, size * count);
}
