#include <fs.h>
#include <devfs.h>
#include <memory.h>
#include <efifo.h>
#include <print.h>

#define BUFSIZ 2048

extern
EFIFO keybd_efifo;

/*static
int linebuf_read(FILE *f, char *buf, size_t size)
{
	unsigned long i = 0;
	while (i < size) {
		*buf++ = *f->f_linebuf.fe_buf[i++];
	}

	return size;
}*/

static
unsigned int linebuf_getch(FILE *f)
{
	if (f->f_linebuf.fe_i == -1) {
		int i;
		for (i = 0; i < BUFSIZ; i++) {
			f->f_linebuf.fe_buf[i++] = getch(f->f_linebuf.fe_fin);
		}
		f->f_linebuf.fe_buf[i] = 0;
		f->f_linebuf.fe_i = 0;
	}
	return f->f_linebuf.fe_buf[f->f_linebuf.fe_i++];
}

static
FILE_OPS linebuf_fops;

static
int linebuf_open(FILE *f, INODE *inode, unsigned int oattr)
{
	f->f_ops = &linebuf_fops;
	f->f_inode = inode;
	f->f_oattr = oattr;

	f->f_linebuf.fe_buf = kmalloc(BUFSIZ + 1);
	f->f_linebuf.fe_fin = kmalloc(FILE);
	inode_open(f->f_line_buf.fe_fin, inode, oattr);

	f->f_pos = 0;
	f->f_size = -1;

	return 0;
}

static
int linebuf_close(__attribute__((unused)) FILE *f)
{
	close(f->f_linebuf.fe_fin);
	kfree(f->f_linebuf.fe_fin);
	kfree(f->f_linebuf.fe_buf);

	return 0;
}

static
FILE_OPS linebuf_fops = {
	.open = linebuf_open,
	.read = linebuf_read,
	.close = linebuf_close,
};


void init_linebuf(void)
{
	register_dev(&linebuf_fops, "stdin", INODE_CHR | INODE_RD);
}
