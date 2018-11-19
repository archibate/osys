#pragma once

#include <stddef.h>
#include <struct.h>

STRUCT(FILE) {
	int f_fd;
	struct FIFO *f_linebuf;
};

extern FILE *stdin, *stdout, *stderr;

FILE *fopen(const char *name, const char *type);
int fopen_i(FILE *f, const char *name, const char *type);
int fopen_s(FILE **pf, const char *name, const char *type);
int fclose_i(FILE *f);
int fclose(FILE *f);
