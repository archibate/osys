#include <fs.h>
#include <efifo.h>
#include <kmalloc.h>
#include <devfs.h>
#include <sched.h>//


static
int mkef_putch(FILE *f, unsigned char ch)
{
	efifo_put(f->f_mkef.fe_efifo, ch);
	return 0;
}

static
int mkef_flush(FILE *f)
{
	efifo_flush(f->f_mkef.fe_efifo);
	return 0;
}

static
unsigned int mkef_getch(FILE *f)
{
	return efifo_wait_get(f->f_mkef.fe_efifo);
}

static
char *mkef_getline(FILE *f)
{
	size_t size = efifo_wait_size(f->f_mkef.fe_efifo);
	char *buf = kmalloc(size + 1);
	__efifo_readline(f->f_mkef.fe_efifo, buf, size);
	buf[size] = 0;
	return buf;
}

static
size_t mkef_glinesize(FILE *f)
{
	return efifo_wait_size(f->f_mkef.fe_efifo);
}

static
int mkef_read(FILE *f, char *buf, size_t size)
{
	int maxsize = efifo_wait_size(f->f_mkef.fe_efifo);
	if (size > maxsize)
		size = maxsize;
	__efifo_readline(f->f_mkef.fe_efifo, buf, size);
	return size;
}

static
FILE_OPS mkef_fops, mkef_e_fops;

static
int mkef_open(FILE *f, INODE *inode, unsigned int oattr)
{
	f->f_ops = &mkef_fops;
	f->f_oattr = oattr;
	f->f_inode = inode;
	f->f_pos = 0;
	f->f_size = -1;
	f->f_mkef.fe_efifo = kmalloc_for(EFIFO);
	efifo_init(f->f_mkef.fe_efifo);
	return 0;
}

static
int mkef_close(FILE *f)
{
	kfree(f->f_mkef.fe_efifo);
	return 0;
}

static
int mkef_e_open(FILE *f, INODE *inode, unsigned int oattr)
{
	f->f_ops = &mkef_fops;
	f->f_oattr = oattr;
	f->f_inode = inode;
	f->f_pos = 0;
	f->f_size = -1;
	f->f_mkef.fe_efifo = inode->i_mkef.ie_efifo;
	return 0;
}

static
FILE_OPS mkef_fops = {
	.open = mkef_open,
	.glinesize = mkef_glinesize,
	.getline = mkef_getline,
	.read = mkef_read,
	.close = mkef_close,
	.putch = mkef_putch,
	.getch = mkef_getch,
};

static
FILE_OPS mkef_e_fops = {
	.open = mkef_e_open,
	// TODO: below really needs under `f->f_ops = &mkef_fops;` had in mkef_e_open?
	.glinesize = mkef_glinesize,
	.getline = mkef_getline,
	.read = mkef_read,
	.close = mkef_close,
	.putch = mkef_putch,
	.getch = mkef_getch,
	.flush = mkef_flush,
};

INODE *make_efifo_dev(const char *name, EFIFO *efifo, unsigned int iattr)
{
	INODE *inode = register_dev(&mkef_fops, name, iattr);
	if (efifo) {
		inode->i_fops = &mkef_e_fops;
		inode->i_mkef.ie_efifo = efifo;
	} else {
		inode->i_fops = &mkef_fops;
	}
	return inode;
}
