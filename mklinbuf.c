#include <fs.h>
#include <devfs.h>
#include <memory.h>
#include <kmalloc.h>
#include <print.h>
#include <mklinbuf.h>

static
FILE_OPS mklb_fops;

static
int mklb_open(FILE *f, INODE *inode, unsigned int oattr)
{
	f->f_ops = &mklb_fops;
	f->f_inode = inode;
	f->f_oattr = oattr;

	f->f_pos = 0;
	f->f_size = -1;

	f->f_mklb.fe_fifo = kmalloc_for(FIFO);
	fifo_init(f->f_mklb.fe_fifo);
	f->f_mklb.fe_flush_cb = inode->i_mklb.ie_flush_cb;

	return 0;
}

static
int mklb_close(FILE *f)
{
	kfree(f->f_mklb.fe_fifo);

	return 0;
}

static
int mklb_flush(FILE *f)
{
	return f->f_mklb.fe_flush_cb(f->f_mklb.fe_fifo);
}

static
int mklb_putch(FILE *f, unsigned char ch)
{
	fifo_put(f->f_mklb.fe_fifo, ch);
	if (ch == '\n')
		return mklb_flush(f);
	return 0;
}

static
int mklb_write(FILE *f, const char *buf, size_t size)
{
	int i = 0;
	while (i++ < size)
		mklb_putch(f, *buf++);

	return size;
}

static
FILE_OPS mklb_fops = {
	.open = mklb_open,
	.putch = mklb_putch,
	.write = mklb_write,
	.flush = mklb_flush,
	.close = mklb_close,
};


INODE *make_linebuf_dev
	( const char *name
	, int (*flush_cb)(FIFO *fifo)
	, unsigned int iattr
	)
{
	INODE *inode = register_dev(&mklb_fops, name, iattr);
	inode->i_mklb.ie_flush_cb = flush_cb;
	return inode;
}
