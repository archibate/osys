#include <fs.h>
#include <print.h>
#include <txtdev.h>
#include <devfs.h>
#include <memory.h>

static
int txtdev_read(FILE *f, char *buf, unsigned long size)
{
	if (size < 0)
		size = 0;
	if (f->f_pos + size > f->f_size)
		size = f->f_size - f->f_pos;
	memcpy(buf, f->f_inode->ie_priv_data + f->f_pos, size);
	f->f_pos += size;
	return size;
}

FILE_OPS txtdev_fops = {
	.open = simple_open,
	.read = txtdev_read,
	.close = simple_close,
};


void setup_txtinfo_dev
	( INODE *inode
	, const char *data
	, size_t size
	)
{
	inode->i_fops = &txtdev_fops;
	inode->ie_priv_data = (void *) data;
	inode->i_size = size;
}
