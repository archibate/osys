#include <irq.h> // IRQ_MAX

void __attribute__((weak)) do_timer
(void)
{
	irq_done(IRQ_TIMER);
}

void __attribute__((weak)) do_keyboard
(void)
{
	irq_done(IRQ_KEYBOARD);
}

void __attribute__((weak)) do_slavepic
(void)
{
	irq_done(IRQ_SLAVEPIC);
}

void __attribute__((weak)) do_3
(void)
{
	irq_done(IRQ_3);
}

void __attribute__((weak)) do_serial
(void)
{
	irq_done(IRQ_SERIAL);
}

void __attribute__((weak)) do_5
(void)
{
	irq_done(IRQ_5);
}

void __attribute__((weak)) do_6
(void)
{
	irq_done(IRQ_6);
}

void __attribute__((weak)) do_7
(void)
{
	irq_done(IRQ_7);
}

void __attribute__((weak)) do_8
(void)
{
	irq_done(IRQ_8);
}

void __attribute__((weak)) do_9
(void)
{
	irq_done(IRQ_9);
}

void __attribute__((weak)) do_10
(void)
{
	irq_done(IRQ_10);
}

void __attribute__((weak)) do_11
(void)
{
	irq_done(IRQ_11);
}

void __attribute__((weak)) do_mouse
(void)
{
	irq_done(IRQ_MOUSE);
}

void __attribute__((weak)) do_13
(void)
{
	irq_done(IRQ_13);
}

void __attribute__((weak)) do_harddisk
(void)
{
	irq_done(IRQ_HARDDISK);
}

void __attribute__((weak)) do_15
(void)
{
	irq_done(IRQ_15);
}

void *irq_table[IRQ_MAX] = {
	do_timer,
	do_keyboard,
	do_slavepic,
	do_3,
	do_serial,
	do_5,
	do_6,
	do_7,
	do_8,
	do_9,
	do_10,
	do_11,
	do_mouse,
	do_13,
	do_harddisk,
	do_15,
};
