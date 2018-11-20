#pragma once


#include <regs.h>
#include <struct.h>
#include <pcbdefs.h>

STRUCT(PCB)
{
	KS_REGS *sp;

	unsigned long *pgd;

	unsigned long brk;

	union {
		struct FILE *files[FILES_MAX];
		struct DIR *dirs[FILES_MAX];
	};
};
