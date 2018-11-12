#include <irq.h>
#include <idt.h> // IRQ_IDT_BEG
#include <ioport.h>


void init_pic
(void)
{
	io_outb(0x20, 0x11);
	io_outb(0xa0, 0x11);

	io_outb(0x21, IRQ_IDT_BEG);
	io_outb(0xa1, IRQ_IDT_BEG + 8);

	io_outb(0x21, 0x04);
	io_outb(0xa1, 0x02);

	io_outb(0x21, 0x01);
	io_outb(0xa1, 0x01);

	io_outb(0x21, 0xfb); // INTR from slave PIC not masked
	io_outb(0xa1, 0xff);
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
