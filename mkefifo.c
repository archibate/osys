#include <fs.h>
#include <efifo.h>
#include <kmalloc.h>
#include <devfs.h>


static
int efifo_putch(FILE *f, unsigned char ch)
{
	efifo_put(f->f_efifo.fe_efifo, ch);
	return 0;
}

static
unsigned int efifo_getch(FILE *f)
{
	return efifo_get(f->f_efifo.fe_efifo);
}

static
FILE_OPS efifo_fops, efifo_e_fops;

static
int efifo_open(FILE *f, INODE *inode, unsigned int oattr)
{
	f->f_ops = &efifo_fops;
	f->f_oattr = oattr;
	f->f_inode = inode;
	f->f_pos = 0;
	f->f_size = -1;
	f->f_efifo.fe_efifo = kmalloc_for(EFIFO);
	return 0;
}

static
int efifo_close(FILE *f)
{
	kfree(f->f_efifo.fe_efifo);
	return 0;
}

static
int efifo_e_open(FILE *f, INODE *inode, unsigned int oattr)
{
	f->f_ops = &efifo_fops;
	f->f_oattr = oattr;
	f->f_inode = inode;
	f->f_pos = 0;
	f->f_size = -1;
	f->f_efifo.fe_efifo = inode->i_mkefifo.ie_efifo;
	return 0;
}

static
int efifo_e_close(__attribute__((unused)) FILE *f)
{
	return 0;
}

static
FILE_OPS efifo_fops = {
	.open = efifo_open,
	.close = efifo_close,
	.putch = efifo_putch,
	.getch = efifo_getch,
};

static
FILE_OPS efifo_e_fops = {
	.open = efifo_e_open,
	.close = efifo_e_close,
	.putch = efifo_putch,
	.getch = efifo_getch,
};

INODE *make_efifo_dev(const char *name, EFIFO *efifo, unsigned int iattr)
{
	INODE *inode = register_dev(&efifo_fops, name, iattr);
	if (efifo) {
		inode->i_fops = &efifo_e_fops;
		inode->i_mkefifo.ie_efifo = efifo;
	} else {
		inode->i_fops = &efifo_fops;
	}
	return inode;
}
