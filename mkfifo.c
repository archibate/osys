#include <fs.h>
#include <fifo.h>
#include <kmalloc.h>
#include <devfs.h>
#include <sched.h>//


static
int mkf_putch(FILE *f, unsigned char ch)
{
	fifo_put(f->f_mkf.fe_fifo, ch);
	return 0;
}

static
unsigned int mkf_getch(FILE *f)
{
	return fifo_get(f->f_mkf.fe_fifo);
}

static
char *mkf_getline(FILE *f)
{
	size_t size = fifo_wait_size(f->f_mkf.fe_fifo);
	char *buf = kmalloc(size + 1);
	__fifo_readline(f->f_mkf.fe_fifo, buf, size);
	buf[size] = 0;
	return buf;
}

static
size_t mkf_glinesize(FILE *f)
{
	return fifo_size(f->f_mkf.fe_fifo);
}

static
int mkf_read(FILE *f, char *buf, size_t size)
{
	int maxsize = fifo_size(f->f_mkf.fe_fifo);
	if (size > maxsize)
		size = maxsize;
	for (int i = 0; i < size; i++) {
		buf[i] = fifo_get(f->f_mkf.fe_fifo);
	}
	return size;
}

static
FILE_OPS mkf_fops, mkf_e_fops;

static
int mkf_open(FILE *f, INODE *inode, unsigned int oattr)
{
	f->f_ops = &mkf_fops;
	f->f_oattr = oattr;
	f->f_inode = inode;
	f->f_pos = 0;
	f->f_size = -1;
	f->f_mkf.fe_fifo = kmalloc_for(fifo);
	fifo_init(f->f_mkf.fe_fifo);
	return 0;
}

static
int mkf_close(FILE *f)
{
	kfree(f->f_mkf.fe_fifo);
	return 0;
}

static
int mkf_e_open(FILE *f, INODE *inode, unsigned int oattr)
{
	f->f_ops = &mkf_fops;
	f->f_oattr = oattr;
	f->f_inode = inode;
	f->f_pos = 0;
	f->f_size = -1;
	f->f_mkf.fe_fifo = inode->i_mkf.ie_fifo;
	return 0;
}

static
FILE_OPS mkf_fops = {
	.open = mkf_open,
	.glinesize = mkf_glinesize,
	.getline = mkf_getline,
	.read = mkf_read,
	.close = mkf_close,
	.putch = mkf_putch,
	.getch = mkf_getch,
};

static
FILE_OPS mkf_e_fops = {
	.open = mkf_e_open,
	// TODO: below really needs under `f->f_ops = &mkf_fops;` had in mkf_e_open?
	.glinesize = mkf_glinesize,
	.getline = mkf_getline,
	.read = mkf_read,
	.close = mkf_close,
	.putch = mkf_putch,
	.getch = mkf_getch,
	.fsync = mkf_fsync,
};

INODE *make_fifo_dev(const char *name, FIFO *fifo, unsigned int iattr)
{
	INODE *inode = register_dev(&mkf_fops, name, iattr);
	if (fifo) {
		inode->i_fops = &mkf_e_fops;
		inode->i_mkf.ie_fifo = fifo;
	} else {
		inode->i_fops = &mkf_fops;
	}
	return inode;
}
