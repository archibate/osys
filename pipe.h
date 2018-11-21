#pragma once

#include <struct.h>

STRUCT(PIPE)
{
	char buf[PIPE_SIZE];
	TCB *reader, *writer;
};

void pipe_write(PIPE *pi, const char *buf, unsigned long n);
