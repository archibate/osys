#include <fs.h>
#include <devfs.h>
#include <memory.h>
#include <mkefifo.h>
#include <print.h>

extern
EFIFO keybd_efifo;


void init_kbd(void)
{
	INODE *inode = dir_new_entry(dev_super->s_inode, "kbd0", INODE_CHR | INODE_RD);
	setup_efifo_dev(inode, &keybd_efifo);
}
