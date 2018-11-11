#include "irq.h" // IRQ_MAX

void *irq_table[IRQ_MAX] = {
	do_timer,
	do_keyboard,
	do_2,
	do_3,
	do_4,
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
        wr
