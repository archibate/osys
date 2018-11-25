#include <fs.h>
#include <print.h>
#include <efifo.h>
#include <kmalloc.h>
#include <devfs.h>
#include <sched.h>//


static
int mkef_putch(FILE *f, unsigned char ch)
{
	efifo_put(f->fe_efifo, ch);
	efifo_flush(f->fe_efifo);
	return 0;
}

/*EFIFO *need_flush_efifo;

static
void will_flush_efifo(EFIFO *efifo)
{
	if (need_flush_efifo)
		efifo_flush(need_flush_efifo);
	need_flush_efifo = efifo;
}*/

static
int mkef_write(FILE *f, const char *buf, size_t size)
{
	int i = 0;
	while (i++ < size) {
		efifo_put(f->fe_efifo, *buf++);
	}
	//will_flush_efifo(f->fe_efifo);
	efifo_flush(f->fe_efifo);
	return i;
}

static
unsigned int mkef_getch(FILE *f)
{
	return efifo_wait_get(f->fe_efifo);
}

static
char *mkef_getline(FILE *f)
{
	size_t size = efifo_wait_size(f->fe_efifo);
	char *buf = kmalloc(size + 1);
	__efifo_readline(f->fe_efifo, buf, size);
	buf[size] = 0;
	return buf;
}

static
size_t mkef_glinesize(FILE *f)
{
	return efifo_wait_size(f->fe_efifo);
}

static
int mkef_read(FILE *f, char *buf, size_t size)
{
	int maxsize = efifo_wait_size(f->fe_efifo);
	if (size > maxsize)
		size = maxsize;
	__efifo_readline(f->fe_efifo, buf, size);
	return size;
}

static
FILE_OPS mkef_fops;

static
int mkef_close(FILE *f)
{
	return 0;
}

static
int mkef_open(FILE *f, INODE *inode, unsigned int oattr)
{
	f->f_ops = &mkef_fops;
	f->f_oattr = oattr;
	f->f_inode = inode;
	f->f_pos = 0;
	f->f_size = -1;
	f->fe_efifo = inode->ie_efifo;
	return 0;
}

static
FILE_OPS mkef_fops = {
	.open = mkef_open,
	.glinesize = mkef_glinesize,
	.getline = mkef_getline,
	.write = mkef_write,
	.read = mkef_read,
	.close = mkef_close,
	.putch = mkef_putch,
	.getch = mkef_getch,
};

INODE *setup_efifo_dev(INODE *inode, EFIFO *efifo)
{
	inode->i_fops = &mkef_fops;
	inode->ie_efifo = efifo;
	return inode;
}
