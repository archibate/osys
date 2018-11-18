#pragma once


#include <regs.h>
#include <struct.h>
#include <pcbdefs.h>

STRUCT(PCB)
{
	KS_REGS *sp;

	struct FILE *files[FILES_MAX];
};
