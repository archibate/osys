#include <fs.h>
#include <devfs.h>
#include <memory.h>
#include <efifo.h>
#include <print.h>

extern
EFIFO keybd_efifo;

static
unsigned int kbd_getch(__attribute__((unused)) FILE *f)
{
	return efifo_get(&keybd_efifo);
}

static
int kbd_read(FILE *f, char *buf, size_t size)
{
	unsigned long i = 0;
	while (i++ < size) {
		*buf++ = kbd_getch(f);
	}

	return size;
}

static
FILE_OPS kbd_fops;

static
int kbd_open(FILE *f, INODE *inode, unsigned int oattr)
{
	f->f_ops = &kbd_fops;
	f->f_inode = inode;
	f->f_oattr = oattr;

	f->f_pos = 0;
	f->f_size = 1;

	return 0;
}

static
int kbd_close(__attribute__((unused)) FILE *f)
{
	return 0;
}

static
FILE_OPS kbd_fops = {
	.open = kbd_open,
	.getch = kbd_getch,
	.read = kbd_read,
	.close = kbd_close,
};


void init_kbd(void)
{
	register_dev(&kbd_fops, "kbd", INODE_CHR | INODE_RD);
}
