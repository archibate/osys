#include <vfs.h>
#include <mkefifo.h>

extern
EFIFO keybd_efifo;


void init_kbd(void)
{
	INODE *inode = dir_new_inode(vfs_inode("/dev"), "kbd0", INODE_CHR | INODE_RD);
	setup_efifo_dev(inode, &keybd_efifo);
	close_inode(inode);
}
