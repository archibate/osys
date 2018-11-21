#include <pipe.h>
#include <devfs.h>


static
int pipf_write(FILE *f, const char *buf, size_t size)
{
	pipe_write(f->f_inode->ie_pipe, buf, size, 0);
	return size;
}

static
int pipf_read(FILE *f, char *buf, size_t size)
{
	return pipe_read(f->f_inode->ie_pipe, buf, size, 0);
}

static
FILE_OPS pipf_fops;

static
int pipf_open(FILE *f, INODE *inode, unsigned int oattr)
{
	f->f_ops = &pipf_fops;
	f->f_oattr = oattr;
	f->f_inode = inode;
	f->f_pos = 0;
	f->f_size = -1;
	return 0;
}

static
unsigned int pipf_getch(FILE *f)
{
	char res;
	pipf_read(f, &res, 1);
	return res;
}

static
int pipf_putch(FILE *f, unsigned char ch)
{
	pipf_write(f, (char*)&ch, 1);
	return 0;
}

static
int pipf_close(FILE *f)
{
	return 0;
}

static
FILE_OPS pipf_fops = {
	.open = pipf_open,
	.read = pipf_read,
	.write = pipf_write,
	.getch = pipf_getch,
	.putch = pipf_putch,
	.close = pipf_close,
};

INODE *make_pipe_dev(const char *name, PIPE *pipe, unsigned int iattr)
{
	INODE *inode = register_dev(&pipf_fops, name, iattr);

	inode->i_fops = &pipf_fops;
	inode->ie_pipe = pipe;

	return inode;
}
