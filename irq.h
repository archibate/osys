#pragma once


#include "gen.irqentinfo.h" // IRQ_MAX
#include "gen.irqidxs.h" // IRQ_*
#include "ioport.h"


extern
void *irq_table[IRQ_MAX];


extern
void set_irq_enable
	( unsigned int irq_no
	, int enable
	);


static inline
void irq_done
	( unsigned int irq_no
	)
{
	io_outb(0x20, 0x20);
	if (irq_no >= 8)
		io_outb(0xa0, 0x20);
}
