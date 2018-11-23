#include <devfs.h>
#include <txtdev.h>
#include <string.h>


void init_welcome(void)
{
	static const char welcome[] = "\n\nOSYS kernel v0.1 (C) archibate 2018\n\n";
	INODE *inode = dir_new_entry(dev_super->s_inode, "welcome", INODE_CHR | INODE_RD);
	setup_txtinfo_dev(inode, welcome, strlen(welcome));
}
