#include <irq.h> // set_irq_enable
#include <kbc.h> // KB_PORT_DATA
#include <ioport.h>
#include <onkeybd.h>


void init_keyboard
(void)
{
	set_irq_enable(IRQ_KEYBOARD, 1);
}


void do_keyboard_data
(unsigned char scancode)
{
	on_keyboard_event(scancode);
}


void do_keyboard
(void)
{
	unsigned char scancode = io_inb(KB_PORT_DATA);
	do_keyboard_data(scancode);

	irq_done(IRQ_KEYBOARD);
}
