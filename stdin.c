#include <fs.h>
#include <devfs.h>
#include <memory.h>
#include <efifo.h>
#include <print.h>

#define BUFSIZ 2048

extern
EFIFO keybd_efifo;

static
int stdin_read(FILE *f, char *buf, size_t size)
{
	unsigned long i = 0;
	while (i++ < size) {
		*buf++ = *f->f_stdin.fe_buf;
	}

	return size;
}

static
FILE_OPS stdin_fops;

static
int stdin_open(FILE *f, INODE *inode, unsigned int oattr)
{
	f->f_ops = &stdin_fops;
	f->f_inode = inode;
	f->f_oattr = oattr;

	f->f_stdin.fe_buf = kmalloc(BUFSIZ);

	f->f_pos = 0;
	f->f_size = -1;

	return 0;
}

static
int stdin_close(__attribute__((unused)) FILE *f)
{
	close(f->f_stdin.fe_fkbd);
	kfree(f->f_stdin.fe_fkbd);
	kfree(f->f_stdin.fe_buf);

	return 0;
}

static
FILE_OPS stdin_fops = {
	.open = stdin_open,
	.read = stdin_read,
	.close = stdin_close,
};


void init_stdin(void)
{
	register_dev(&stdin_fops, "stdin", INODE_CHR | INODE_RD);
}
