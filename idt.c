#include "idt.h"


struct GATE_DESC
{
	unsigned off_low  : 16;
	unsigned seg_sel  : 16;
	unsigned args_nr  : 5;
	unsigned reserved : 3;
	unsigned type     : 4;
	unsigned system   : 1;
	unsigned dpl      : 2;
	unsigned present  : 1;
	unsigned off_high : 16;
} __attribute__((packed));


#define AVAL_TSS  0x9
#define INTR_GATE 0xe
#define TRAP_GATE 0xf


static
void set_gatedesc
	( struct GATE_DESC *desc
	, unsigned short seg_sel
	, unsigned long offset
	, int dpl
	)
{
	desc->reserved = 0;
	desc->args_nr  = 0;
	desc->present  = 1;
	desc->system   = 0;
	desc->type     = (dpl & 4) ? TRAP_GATE : INTR_GATE;
	desc->dpl      = dpl & 3;
	desc->seg_sel  = seg_sel;
	desc->off_low  = offset & 0xffff;
	desc->off_high = (offset >> 16) & 0xffff;
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



static
void fill_up_idt
(void);


static
void load_up_idtr
(void)
{
	struct
	{
		unsigned short limit;
		unsigned long base;
	} __attribute__((packed)) idtr = {
		8 * IDT_MAX - 1, (unsigned long) idt,
	};

	asm volatile ("lidt %0" :: "m" (idtr));
}


void init_idt
(void)
{
	fill_up_idt();
	load_up_idtr();
}


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


void fill_up_idt
(void)
{
	fill_with(exp_ents,           0, EXP_MAX);
	fill_with(irq_ents, IRQ_IDT_BEG, IRQ_MAX);
}
