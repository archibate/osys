#pragma once

#include <struct.h>

#define EXF_MAGIC 0xCAFEBABE

STRUCT(EXF_HEADER)
{
	unsigned int x_magic;
	unsigned int x_stkpgs;
	off_t x_off;
	size_t x_size;
};
