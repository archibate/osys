#include <mklinbuf.h>
#include <serial.h>

static
int mon_flush_cb(FIFO *fifo)
{
	int size = fifo_size(fifo);
	while (size--)
		serial_putc(fifo_get(fifo));
	return 0;
}

void init_mon(void)
{
	make_linebuf_dev("mon0", mon_flush_cb, INODE_CHR | INODE_WR);
}
