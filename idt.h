#pragma once


#include "gen.irqentinfo.h" // IRQ_MAX
#include "gen.expentinfo.h" // EXP_MAX


#define IDT_MAX 0x100
#define IRQ_IDT_BEG 0x20


extern
void set_idt
	( unsigned int idx
	, void *addr
	, int dpl
	);
