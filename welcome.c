#include <fs.h>
#include <devfs.h>
#include <memory.h>

static const char welcome[] = "\n\nOSYS kernel v0.1 (C) archibate 2018\n\n";


static
FILE_OPS welcome_fops;

static
int welcome_open(FILE *f, INODE *inode, unsigned int oattr)
{
	f->f_ops = &welcome_fops;
	f->f_inode = inode;
	f->f_oattr = oattr;

	f->f_pos = 0;
	f->f_size = sizeof(welcome);

	return 0;
}

static
int welcome_close(__attribute__((unused)) FILE *f)
{
	return 0;
}

static
int welcome_read(FILE *f, char *buf, size_t size)
{
	if (size < 0)
		size = 0;
	if (f->f_pos + size > sizeof(welcome))
		size = sizeof(welcome) - f->f_pos;
	memcpy(buf, welcome + f->f_pos, size);
	f->f_pos += size;
	return size;
}

static
FILE_OPS welcome_fops = {
	.open = welcome_open,
	.read = welcome_read,
	.close = welcome_close,
};


void init_welcome(void)
{
	register_dev(&welcome_fops, "welcome", INODE_CHR | INODE_RD);
}
