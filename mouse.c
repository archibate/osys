#include <kbc.h> // kbc_cmd, KB*
#include <irq.h> // set_irq_enable
#include <onmouse.h> // on_mouse_event
#include <ioport.h>


static unsigned char mdat0, mdat1;
static int phase;


void init_mouse
(void)
{
	kbc_cmd(KB_CMD_TO_MOUSE, KB_MOUSE_CMD_ENABLE);
	phase = 0;

	set_irq_enable(IRQ_MOUSE, 1);
}


void do_mouse_data
(unsigned char a)
{
	if (phase == 0)
	{
		if (a == 0xfa)
			phase = 1;
	}
	else if (phase == 1)
	{
		if ((a & 0xc8) != 0x08)
			return;

		phase = 2;
		mdat0 = a;
	}
	else if (phase == 2)
	{
		phase = 3;
		mdat1 = a;
	}
	else if (phase == 3)
	{
		phase = 1;

		int mdy = a, mdx = mdat1;
		int mbtn = mdat0 & 0x07;

		if (mdat0 & 0x10)
			mdx |= 0xffffff00;

		if (mdat0 & 0x20)
			mdy |= 0xffffff00;

		mdy = -mdy;

		on_mouse_event(mbtn, mdx, mdy);
	}
}


void do_mouse
(void)
{
	irq_done(IRQ_MOUSE);

	unsigned char a = io_inb(KB_PORT_DATA);
	do_mouse_data(a); // TODO: use a FIFO buffer
}
