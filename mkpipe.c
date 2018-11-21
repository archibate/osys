#include <fs.h>
#include <pipe.h>
#include <kmalloc.h>
#include <devfs.h>


static
int pip_write(FILE *f, const char *buf, size_t size)
{
	int i = 0;
	pipe_write(f->f_pip.fe_pipe, buf, size);
	return i;
}

static
int pip_read(FILE *f, char *buf, size_t size)
{
	pipe_read(f->f_pip.fe_pipe, buf, size);
	return size;
}

static
FILE_OPS pip_fops, pip_e_fops;

static
int pip_close(FILE *f)
{
	return 0;
}

static
int pip_open(FILE *f, INODE *inode, unsigned int oattr)
{
	f->f_ops = &pip_fops;
	f->f_oattr = oattr;
	f->f_inode = inode;
	f->f_pos = 0;
	f->f_size = -1;
	f->f_pipe = inode->i_pip.ie_pipe;
	return 0;
}

static
FILE_OPS pip_fops = {
	.open = pip_open,
	.write = pip_write,
	.read = pip_read,
	.close = pip_close,
};

INODE *make_pipe_dev(const char *name, PIPE *pipe, unsigned int iattr)
{
	INODE *inode = register_dev(&pip_fops, name, iattr);

	inode->i_fops = &pip_e_fops;
	inode->i_pip.ie_pipe = pipe;

	return inode;
}
