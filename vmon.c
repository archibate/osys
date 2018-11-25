#include <vfs.h>
#include <mkefifo.h>

EFIFO vmon_efifo;


void init_vmon(void)
{
	INODE *inode = dir_new_inode(vfs_inode("/dev"), "vmon0", INODE_CHR | INODE_RD | INODE_WR);
	setup_efifo_dev(inode, &vmon_efifo);
	close_inode(inode);
}
