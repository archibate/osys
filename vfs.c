#include <vfs.h>
#include <print.h>
#include <errno.h>
#include <kmalloc.h>
#include <memory.h>
#include <panic.h>


DIR *root = 0;
DIR *cdir = 0;


int chdir(const char *path)
{
	DIR *dir = kmalloc_for(DIR);
	int res = opendir(dir, path, OPEN_RD | OPEN_WR);
	if (!res) {
		if (cdir)
			kfree(cdir);
		cdir = dir;
	}
	return res;
}


DIRENT *locate_entry(const char *name)
{
	DIR *dir = cdir;
	if (name[0] == '/') {
		dir = root;
		name++;
	}
	return dir_locate_entry(dir, name);
}


void vfs_mount(const char *name, SUPER *sb)
{
	DIRENT *de = locate_entry(name);
	/*if (de->e_inode)
		free_inode(de->e_inode);//FIXME*/
	de->e_sb = sb;
	de->e_ino = 0;
}


int open(FILE *file, const char *name, unsigned int oattr)
{
	if (name[0] == '.' && name[1] == 0)
		return inode_open(file, cdir->d_inode, oattr);
	if (name[0] == '/' && name[1] == 0)
		return inode_open(file, root->d_inode, oattr);

	DIRENT *de = locate_entry(name);
	if (!de)
		return -ENOENT;

	return inode_open(file, open_dirent(de), oattr);
}
