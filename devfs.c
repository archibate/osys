#include <vfs.h>
#include <kmalloc.h>
#include <print.h>
#include <string.h>


INODE_OPS devfs_iops = {
};


static
INODE *devfs_alloc_inode(SUPER *sb)
{
	INODE *inode = kmalloc_for(INODE);
	inode->i_sb = sb;
	inode->i_ops = &devfs_iops;
	return inode;
}

static
void devfs_free_inode(INODE *inode)
{
	kfree(inode);
}

static
void devfs_free_super(SUPER *sb)
{
	for (LIST *le = sb->s_root->d_ents; le;) {
		DIRENT *de = list_entry(DIRENT, e_list, le);
		le = le->next;
		free_inode(de->e_inode);
		kfree(de);
	}
	kfree(sb->s_root);
	kfree(sb);
}

static
SUPER_OPS devfs_super_ops = {
	.free_super = devfs_free_super,
	.alloc_inode = devfs_alloc_inode,
	.free_inode = devfs_free_inode,
};


static
DIR_OPS devfs_devdir_dops;

int devfs_devdir_closedir(DIR *dir)
{
	return 0;
}

int devfs_devdir_opendir(DIR *dir, INODE *inode, unsigned int oattr)
{
	dir->d_ops = &devfs_devdir_dops;
	dir->d_ents = inode->ie_dents;
	return 0;
}

static
DIR_OPS devfs_devdir_dops = {
	.opendir = devfs_devdir_opendir,
	.closedir = devfs_devdir_closedir,
	.dirfind = simple_dirfind,
};

static
SUPER *devfs_load_super
(void *__attribute__((unused)) unused)
{
	SUPER *sb = kmalloc_for(SUPER);
	sb->s_ops = &devfs_super_ops;
	sb->s_inode = alloc_inode(sb);
	sb->s_inode->i_attr = INODE_RD | INODE_WR;
	sb->s_inode->i_type = INODE_DIR;
	sb->s_inode->i_dops = &devfs_devdir_dops;
	sb->s_inode->ie_dents = 0;

	return sb;
}

static
FSDRIVE devfs_drive = {
	.name = "devfs",
	.load_super = devfs_load_super,
};




SUPER *dev_super;

INODE *register_dev
	( FILE_OPS *fops
	, const char *name
	, unsigned int iattr
	)
{
	DIRENT *de = kmalloc_for(DIRENT);
	strcpy(de->e_name, name);

	de->e_inode = alloc_inode(dev_super);
	de->e_inode->i_attr = iattr;
	de->e_inode->i_fops = fops;

	list_add_head_n(&dev_super->s_inode->ie_dents, &de->e_list);

	return de->e_inode;
}

void init_devfs(void)
{
	add_fsdrive(&devfs_drive);

	dev_super = load_super("devfs", 0);

	vfs_mount("/dev", dev_super);
}
