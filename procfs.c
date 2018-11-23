#include <devfs.h>
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

	DIRENT *de = __dir_new_entry(proc_super->s_inode, 0, iattr);
	escape_fname_to(de->e_name, tcb->name);
	inode = de->e_inode;
	inode->i_dops = &proc_dops;

	subnode = dir_new_entry(inode, "name", INODE_RD);
	setup_txtinfo_dev(subnode, tcb->name, strlen(tcb->name));

	subnode = dir_new_entry(inode, "pid", INODE_RD);
	setup_numinfo_dev(subnode, pid);

	return inode;
}
