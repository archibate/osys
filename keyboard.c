#include <irq.h> // set_irq_enable
#include <kbc.h> // KB_PORT_DATA
#include <ioport.h>
#include <onkeybd.h>


void init_keyboard
(void)
{
	set_irq_enable(IRQ_KEYBOARD, 1);
}


void do_keyboard
(void)
{
	irq_done(IRQ_KEYBOARD);

	unsigned char scancode = io_inb(KB_PORT_DATA);
	on_keyboard_event(scancode);
}
