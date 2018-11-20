#include <fs.h>
#include <devfs.h>
#include <memory.h>


static
FILE_OPS txtdev_fops;

static
int txtdev_open(FILE *f, INODE *inode, unsigned int oattr)
{
	f->f_ops = &txtdev_fops;
	f->f_inode = inode;
	f->f_oattr = oattr;

	f->f_pos = 0;
	f->f_size = inode->i_size;
	f->f_txtinfo.fe_data = inode->i_txtinfo.ie_data;

	return 0;
}

static
int txtdev_close(__attribute__((unused)) FILE *f)
{
	return 0;
}

static
int txtdev_read(FILE *f, char *buf, unsigned long size)
{
	if (size < 0)
		size = 0;
	if (f->f_pos + size > f->f_size)
		size = f->f_size - f->f_pos;
	memcpy(buf, f->f_txtinfo.fe_data + f->f_pos, size);
	f->f_pos += size;
	return size;
}

static
FILE_OPS txtdev_fops = {
	.open = txtdev_open,
	.read = txtdev_read,
	.close = txtdev_close,
};


INODE *make_txtinfo_dev
	( const char *name
	, const void *data
	, size_t size
	)
{
	INODE *inode = register_dev(&txtdev_fops, name, INODE_CHR | INODE_RD);
	inode->i_txtinfo.ie_data = data;
	inode->i_size = size;
	return inode;
}
