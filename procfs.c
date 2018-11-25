#include <vfs.h>
#include <kmalloc.h>
#include <txtdev.h>
#include <string.h>
#include <sprintf.h>
#include <pid.h>

/*static
void escape_fname_to(char *dst, const char *src)
{
	char c;
	while ((c = *src++)) {
		if (c == '/')
			c = '%';
		*dst++ = c;
	}
}*/

void procfs_new_proc(TCB *tcb, int pid, unsigned int iattr)
{
	INODE *inode, *subnode;
	iattr |= INODE_DIR;

	char *pidstr = kmalloc(12);
	sprintf(pidstr, "%d", pid);

	DIRENT *de = dir_new_entry(vfs_inode("/proc"), pidstr);
	inode = alloc_open_inode(de->e_sb);
	inode->i_attr = iattr;

	subnode = dir_new_inode(inode, "name", INODE_RD);
	setup_strinfo_dev(subnode, tcb->name);
	close_inode(subnode);

	subnode = dir_new_inode(inode, "pid", INODE_RD);
	setup_strinfo_dev(subnode, pidstr);
	close_inode(subnode);

	link(inode, de);
	kfree(pidstr);
}
