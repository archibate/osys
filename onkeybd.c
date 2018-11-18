#include <onkeybd.h>
#include <print.h>
#include <efifo.h>
#include <vkeys.h>


EFIFO keybd_efifo;


_Bool keydowns[0x80];


void on_keyboard_event
	( unsigned char scancode
	)
{
	prrdtsc("K");

	keydowns[scancode & 0x7f] = (scancode & 0x80);

	extern const int keymap[0x80];
	int ch = keymap[scancode];

	if (scancode < 0x80)
	{
		putchar(ch);

		efifo_put(&keybd_efifo, ch);
		if (ch == '\n')
			efifo_flush(&keybd_efifo);
	}
}


/*int onkeybd_daemon
(void *kbd_efifo)
{
	for (;;) {
		if (!fifo_empty(&keybd_fifo))
			efifo_put(kbd_efifo, fifo_get(&keybd_fifo));
		do_schedule();
	}
}

void init_onkeybd
(void)
{
	onkeybd_daemon();
}*/
