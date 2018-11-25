#include <vfs.h>
#include <txtdev.h>
#include <numdev.h>
#include <string.h>
#include <pid.h>

static
DIR_OPS proc_dops = {
	.opendir = simple_opendir,
	.closedir = simple_closedir,
	.dirfind = simple_dirfind,
};

static
void escape_fname_to(char *dst, const char *src)
{
	char c;
	while ((c = *src++)) {
		if (c == '/')
			c = '%';
		*dst++ = c;
	}
}

INODE *procfs_new_proc(TCB *tcb, int pid, unsigned int iattr)
{
	INODE *inode, *subnode;
	iattr |= INODE_DIR;

	DIRENT *de = dir_new_entry(vfs_inode("/proc"), 0);
	escape_fname_to(de->e_name, tcb->name);
	inode = alloc_open_inode(de->e_sb);
	inode->i_dops = &proc_dops;
	inode->i_attr = iattr;
	close_inode(inode);
	link(inode, de);

	subnode = dir_new_inode(inode, "name", INODE_RD);
	setup_txtinfo_dev(subnode, tcb->name, strlen(tcb->name));
	close_inode(subnode);

	subnode = dir_new_inode(inode, "pid", INODE_RD);
	setup_numinfo_dev(subnode, pid);
	close_inode(subnode);

	return inode;
}
