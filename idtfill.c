#include "idt.h"


extern void *exp_ents[EXP_MAX]; // expents.asm
extern void *irq_ents[IRQ_MAX]; // irqents.asm


static
void fill_with
	( void **ents
	, unsigned int beg_idx
	, unsigned int count
	)
{
	for (int i = 0; i < count; i++)
		set_idt(beg_idx + i, ents[i], 0);
}


void init_idtfill
(void)
{
	fill_with(exp_ents,       0, EXP_MAX);
	fill_with(irq_ents, IRQ_BEG, IRQ_MAX);
}
