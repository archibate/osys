#include <fs.h>
#include <devfs.h>
#include <mkefifo.h>
#include <print.h>

EFIFO vmon_efifo;


void init_vmon(void)
{
	INODE *inode = dir_new_entry(dev_super->s_inode, "vmon0", INODE_CHR | INODE_RD | INODE_WR);
	setup_efifo_dev(inode, &vmon_efifo);
}
