#include <fs.h>
#include <devfs.h>
#include <serial.h>
#include <print.h>

static
FILE_OPS mon_fops;

static
int mon_open(FILE *f, INODE *inode, unsigned int oattr)
{
	f->f_ops = &mon_fops;
	f->f_inode = inode;
	f->f_oattr = oattr;

	f->f_pos = 0;
	f->f_size = -1;

	return 0;
}

static
int mon_close(FILE *f)
{
	return 0;
}

static
int mon_putch(FILE *f, unsigned char ch)
{
	serial_putc(ch);
	return 0;
}

static
int mon_write(FILE *f, const char *buf, size_t size)
{
	int i = 0;
	while (i++ < size)
		serial_putc(*buf++);

	return size;
}

static
FILE_OPS mon_fops = {
	.open = mon_open,
	.putch = mon_putch,
	.write = mon_write,
	.close = mon_close,
};

void init_mon(void)
{
	register_dev(&mon_fops, "mon0", INODE_CHR | INODE_WR);
}
