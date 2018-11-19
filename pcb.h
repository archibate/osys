#pragma once


#include <regs.h>
#include <struct.h>
#include <pcbdefs.h>

STRUCT(PCB)
{
	KS_REGS *sp;

	unsigned long *pgd;

	unsigned long brk;

	struct FILE *files[FILES_MAX];
};
