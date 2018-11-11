#pragma once


#include "list.h"
#include "regs.h"
#include "struct.h"


STRUCT(TCB)
{
	LIST list;

	KS_REGS *sp;
};


void do_schedule
(void);

void switch_to
	( TCB *next
	);
