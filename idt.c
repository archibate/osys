#include "idt.h"


struct GATE_DESC
{
	unsigned off_low : 16;
	unsigned seg_sel : 16;
	unsigned mustbe_0 : 7;
	unsigned is_trap : 1;
	unsigned mustbe_7 : 3;
	unsigned dpl : 2;
	unsigned present : 1;
	unsigned off_high : 16;
};


static
void set_gatedesc
	( struct GATE_DESC *desc
	, unsigned short seg_sel
	, unsigned long offset
	, int dpl
	)
{
	desc->mustbe_0 = 0;
	desc->mustbe_7 = 7;
	desc->present  = 1;
	desc->seg_sel  = seg_sel;
	desc->off_low  = offset & 0xffff;
	desc->off_high = (offset >> 16) & 0xffff;
	desc->is_trap  = (dpl & 4) ? 1 : 0;
	desc->dpl      = dpl & 3;
}



struct GATE_DESC idt[IDT_MAX] __attribute__((aligned(8)));


void set_idt
	( unsigned int idx
	, void *addr
	, int dpl
	)
{
	set_gatedesc(&idt[idx], 0x0008, (unsigned long) addr, dpl);
}



void init_idt
(void)
{
	struct
	{
		unsigned short limit;
		unsigned long base;
	} __attribute__((packed)) idtr = {
		IDT_MAX, (unsigned long) idt,
	};

	asm volatile ("lidt %0" :: "m" (idtr));
}
