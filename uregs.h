#pragma once

#include <struct.h>

STRUCT(IF_REGS)
{
	unsigned long di, si, bp, unused_sp, bx, dx, cx, ax;
	unsigned long es, ds;
	unsigned long exp_addr;
	union {
		unsigned long exp_errcd, isr_nr;
	};
	unsigned long pc, cs, eflags, sp, ss;
} __attribute__((packed));

typedef IF_REGS PT_REGS;
