#include <kbc.h>
#include <ioport.h>


static
void kbc_wait_io
(void)
{
	while (io_inb(KB_PORT_STATE) & KB_STATE_NOT_READY)
		;
}


void kbc_cmd
	( unsigned char cmd
	, unsigned char data
	)
{
	kbc_wait_io();
	io_outb(KB_PORT_CMD,  cmd);

	kbc_wait_io();
	io_outb(KB_PORT_DATA, data);
}


void init_kbc
(void)
{
	kbc_cmd(KB_CMD_SET_MODE, KB_MODE_CONTROLLER);
}
