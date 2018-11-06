#include "irq.h"
#include "ioport.h"


void *irq_table[IRQ_MAX];


void init_pic
(void)
{
	io_outb(0x20, 0x11);
	io_outb(0xa0, 0x11);

	io_outb(0x21, 0x20);
	io_outb(0xa1, 0x28);

	io_outb(0x21, 0x04);
	io_outb(0xa1, 0x02);

	io_outb(0x21, 0x01);
	io_outb(0xa1, 0x01);

	io_outb(0x21, 0xff);
	io_outb(0xa1, 0xfb); // slave PIC not masked
}


void set_irq_enable
	( unsigned int irq_no
	, int enable
	)
{
	unsigned char mask;
	unsigned short port = 0x21;

	if (irq_no >= 8) {
		port += 0x80;
		irq_no -= 8;
	}

	mask = io_inb(port);
	if (enable)
		mask &= 0xff ^ (1 << irq_no);
	else
		mask |= 1 << irq_no;
	io_outb(port, mask);
}
