#pragma once

#include <stddef.h>
#include <stdarg.h>
#include <sizet.h>
#include <struct.h>

#define BUFSIZ 1024
typedef unsigned short _fbuf_idx_t;

STRUCT(FILE) {
	int f_fd;
	unsigned int f_oattr;
	_fbuf_idx_t f_bpos, f_bsize;
	char f_buf[BUFSIZ+1];
};

extern FILE *stdin, *stdout, *stderr;

int vprintf(const char *fmt, va_list ap);
int printf(const char *fmt, ...);
int vfprintf(FILE *f, const char *fmt, va_list ap);
int fprintf(FILE *f, const char *fmt, ...);
FILE *fopen(const char *name, const char *type);
int fopen_i(FILE *f, const char *name, const char *type);
int fopen_s(FILE **pf, const char *name, const char *type);
int fread(void *p, size_t size, size_t count, FILE *f);
int fwrite(const void *p, size_t size, size_t count, FILE *f);
int fgets(char *s, size_t size, FILE *f);
const char *fgetrdbuf(FILE *f);
int fputs(const char *s, FILE *f);
int file_need_wr_flush(FILE *f);
void file_wr_flush(FILE *f);
void file_rd_flush(FILE *f);
int fflush(FILE *f);
int fclose_i(FILE *f);
int fclose(FILE *f);

static inline
int __fputc(int c, FILE *f)
{
	return f->f_buf[f->f_bpos++] = c;
}

static inline
int __fgetc(FILE *f)
{
	return f->f_buf[f->f_bpos++];
}

static inline
int fgetc_ex(FILE *f, void (*will_wait_cb)(void))
{
	if (f->f_bpos >= f->f_bsize) {
		will_wait_cb();
		file_rd_flush(f);
	}
	return __fgetc(f);
}

static inline
int fgetc(FILE *f)
{
	if (f->f_bpos >= f->f_bsize)
		file_rd_flush(f);
	return __fgetc(f);
}

static inline
int fputc(int c, FILE *f)
{
	__fputc(c, f);
	if (file_need_wr_flush(f))
		file_wr_flush(f);
	return c;
}

static inline
int putchar(char c)
{
	return fputc(c, stdout);
}

static inline
int getchar(char c)
{
	return fgetc(stdin);
}

static inline
int puts(const char *s)
{
	return fputs(s, stdout);
}
