#include <fs.h>
#include <devfs.h>
#include <map.h>
#include <panic.h>
#include <errno.h>
#include <print.h>



struct VIDEO_INFO
{
	unsigned char *buf;
	unsigned short xsiz;
	unsigned short ysiz;
	unsigned short vmode;
	unsigned char bpp;
	unsigned char mmodel;
} __attribute__((packed)) *video = (struct VIDEO_INFO *) 0x7b00;


static
FILE_OPS vram_fops;

static
int vram_open(FILE *f, INODE *inode, unsigned int oattr)
{
	f->f_ops = &vram_fops;
	f->f_inode = inode;
	f->f_oattr = oattr;

	f->f_pos = 0;
	f->f_size = video->xsiz * video->ysiz;

	return 0;
}

static
int vram_close(__attribute__((unused)) FILE *f)
{
	return 0;
}

static
int vram_mmap(FILE *f, void *p, size_t size, unsigned int mattr)
{
	unsigned long addr = (unsigned long) p;
	assert((addr & PGATTR) == 0);
	size = PGMASK & (PGSIZE - 1 + size);

	unsigned char *src = video->buf + f->f_pos;
	if ((unsigned long) src & PGATTR)
		return -EINVAL;

	int i = 0;
	while (i < size) {
		map(addr, (unsigned long) src | mattr | PG_P);
		addr += PGSIZE;
		src  += PGSIZE;
		i    += PGSIZE;
	}

	return i > size ? size : i;
}

static
FILE_OPS vram_fops = {
	.open = vram_open,
	.mmap = vram_mmap,
	.close = vram_close,
};


void init_vram(void)
{
	register_dev(&vram_fops, "vram", INODE_CHR | INODE_RD | INODE_WR);
}
