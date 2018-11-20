#pragma once

#include <stddef.h>
#include <sizet.h>
#include <struct.h>

STRUCT(FILE) {
	int f_fd;
	//struct FIFO *f_linebuf;
};

extern FILE *stdin, *stdout, *stderr;

FILE *fopen(const char *name, const char *type);
int fopen_i(FILE *f, const char *name, const char *type);
int fopen_s(FILE **pf, const char *name, const char *type);
int fread(void *p, size_t size, size_t count, FILE *f);
int fwrite(const void *p, size_t size, size_t count, FILE *f);
int fputs(const char *s, FILE *f);
int fputc(FILE *f, int c);
int fgetc(FILE *f);
int fclose_i(FILE *f);
int fclose(FILE *f);
int fflush(FILE *f);
