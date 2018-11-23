#include <fs.h>
#include <numdev.h>
#include <devfs.h>
#include <memory.h>
#include <string.h>
#include <sprintf.h>
#include <kmalloc.h>

static
int numdev_read(FILE *f, char *buf, unsigned long size)
{
	if (size < 0)
		size = 0;
	if (f->f_pos + size > f->f_size)
		size = f->f_size - f->f_pos;
	memcpy(buf, f->f_inode->ie_priv_data + f->f_pos, size);
	f->f_pos += size;
	return size;
}

FILE_OPS numdev_fops = {
	.open = simple_open,
	.read = numdev_read,
	.close = simple_close,
};


void setup_numinfo_dev
	( INODE *inode
	, unsigned long num
	)
{
	inode->i_fops = &numdev_fops;
	char *buf = kmalloc(12);
	sprintf(buf, "%d\n", num);
	inode->ie_priv_data = (void *) buf;
	inode->i_size = strlen(buf);
}
