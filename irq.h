#pragma once


#include "gen.irqentinfo.h" // IRQ_MAX
#include "gen.irqidxs.h" // IRQ_*


extern
void *irq_table[IRQ_MAX];


extern
void set_irq_enable
	( unsigned int irq_no
	, int enable
	);
