#pragma once

#include <struct.h>
#include <sizet.h>

#define PIPSIZ 65536
typedef unsigned short pipi_t;

STRUCT(PIPE)
{
	char *buf;
	pipi_t rd, wr;
	struct TCB *reader, *writer;
};

void pipe_init(PIPE *pi);
void pipe_uninit(PIPE *pi);
ssize_t pipe_read(PIPE *pi, char *buf, size_t n, int block);
void pipe_write(PIPE *pi, const char *buf, size_t n, int block);
