#include <devfs.h>
#include <txtdev.h>
#include <vinfo.h>

void init_vinfo(void)
{
	const VIDEO_INFO *vinfo = (const VIDEO_INFO *) 0x7b00;
	INODE *inode = dir_new_entry(dev_super->s_inode, "vinfo", INODE_CHR | INODE_RD);
	setup_txtinfo_dev(inode, (void *) vinfo, sizeof(VIDEO_INFO));
}
