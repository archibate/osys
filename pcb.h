#pragma once


#include <regs.h>
#include <struct.h>

STRUCT(PCB)
{
	KS_REGS *sp;

	unsigned long *pgd;

	const char *name;
};

#include <upcb.h>
