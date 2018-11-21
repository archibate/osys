#include <fs.h>
#include <devfs.h>
#include <mkefifo.h>
#include <print.h>

EFIFO vmon_efifo;


void init_vmon(void)
{
	make_efifo_dev("vmon0", &vmon_efifo, INODE_CHR | INODE_RD | INODE_WR);
}
