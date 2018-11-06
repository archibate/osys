#pragma once


#include "gen.irqentinfo.h" // IRQ_MAX
#include "gen.expentinfo.h" // EXP_MAX


#define IDT_MAX 256
#define IRQ_BEG EXP_MAX


extern
void set_idt
	( unsigned int idx
	, void *addr
	, int dpl
	);
