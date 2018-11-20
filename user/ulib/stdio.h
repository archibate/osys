#pragma once

#include <stddef.h>
#include <stdarg.h>
#include <sizet.h>
#include <struct.h>

#define BUFSIZ 1024
typedef unsigned short _fbuf_idx_t;

STRUCT(FILE) {
	int f_fd;
	char f_buf[BUFSIZ];
	_fbuf_idx_t f_bpos, f_bsize;
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
int fgetline(char *s, size_t size, FILE *f);
int fgetc_ex(FILE *f, void (*will_wait_cb)(void));
int fputs(const char *s, FILE *f);
int fputc(int c, FILE *f);
int fclose_i(FILE *f);
int fclose(FILE *f);
int fflush(FILE *f);

static inline
int fgetc(FILE *f)
{
	return fgetc_ex(f, NULL);
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
