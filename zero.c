#include <fs.h>
#include <devfs.h>
#include <memory.h>


static
FILE_OPS zero_fops;

static
int zero_open(FILE *f, INODE *inode, unsigned int oattr)
{
	f->f_ops = &zero_fops;
	f->f_inode = inode;
	f->f_oattr = oattr;

	f->f_pos = 0;
	f->f_size = -1;

	return 0;
}

static
int zero_close(__attribute__((unused)) FILE *f)
{
	return 0;
}

static
int zero_read(FILE *f, char *buf, size_t size)
{
	memset(buf, 0, size);
	return size;
}

static
int zero_write(FILE *f, const char *buf, size_t size)
{
	return size;
}

static
FILE_OPS zero_fops = {
	.open = zero_open,
	.read = zero_read,
	.write = zero_write,
	.close = zero_close,
};


void init_zero(void)
{
	register_dev(&zero_fops, "zero", INODE_CHR | INODE_RD | INODE_WR);
}
