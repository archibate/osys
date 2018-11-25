#include <vfs.h>
#include <txtdev.h>
#include <string.h>


void init_welcome(void)
{
	INODE *inode = dir_new_inode(vfs_inode("/dev"), "welcome", INODE_CHR | INODE_RD);
	setup_strinfo_dev(inode, "\n\nOSYS kernel v0.1 (C) archibate 2018\n\n");
	close_inode(inode);
}
