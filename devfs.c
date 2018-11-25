#include <vfs.h>
#include <kmalloc.h>
#include <memory.h>
#include <print.h>
#include <panic.h>
#include <string.h>


/*static
INODE_OPS devfs_iops = {
	.link = simple_link,
};

static
DIR_OPS devfs_dops = {
	.opendir = simple_opendir,
	.closedir = simple_closedir,
	.rewinddir = simple_rewinddir,
	.readdir = simple_readdir,
	.dirfind = simple_dirfind,
};


static
ino_t devfs_alloc_ino(SUPER *sb)
{
	ino_t ino = sb->se_now_ino++;
	bzero(sb->se_inodes[ino] = kmalloc_for(INODE), sizeof(INODE));
	sb->se_inodes[ino]->i_ops = &devfs_iops;
	sb->se_inodes[ino]->i_dops = &devfs_dops;
	sb->se_inodes[ino]->i_no = ino;
	sb->se_inodes[ino]->i_sb = sb;
	return ino;
}

static
INODE *devfs_open_inode(SUPER *sb, ino_t ino)
{
	return sb->se_inodes[ino];
}

static
void devfs_free_super(SUPER *sb)
{
	panic("devfs does not support free_super (yet)\n");
}

static
void devfs_free_super(SUPER *sb)
{
	panic("devfs does not support free_super (yet)\n");
	*DIRENT *de;
	DIR dir;
	simple_opendir(&dir, sb->s_inode, OPEN_RD);
	while ((de = readdir(&dir))) {
		free_inode(de->e_inode);
		kfree(de);
	}
	simple_closedir(&dir);
	kfree(sb);*
}

static
SUPER_OPS devfs_super_ops = {
	.free_super = devfs_free_super,
	.alloc_ino = devfs_alloc_ino,
	.open_inode = devfs_open_inode,
	.close_inode = simple_close_inode,
};

static
SUPER *devfs_load_super
(void *__attribute__((unused)) unused)
{
	SUPER *sb = kmalloc_for(SUPER);
	bzero(sb, sizeof(SUPER));
	sb->s_ops = &devfs_super_ops;
	INODE *inode = alloc_open_inode(sb);
	inode->i_attr = INODE_RD | INODE_WR;
	inode->i_type = INODE_DIR;
	inode->i_dops = &devfs_dops;
	inode->ie_dents = 0;

	return sb;
}

static
FSDRIVE devfs_drive = {
	.name = "devfs",
	.load_super = devfs_load_super,
};


SUPER *dev_super, *proc_super;*/

INODE *register_dev
	( FILE_OPS *fops
	, const char *name
	, unsigned int iattr
	)
{
	;
	INODE *inode = dir_new_inode(open_dirent(locate_entry("/dev")), name, iattr);
	inode->i_fops = fops;
	return inode;
}

void init_devfs(void)
{
	/*add_fsdrive(&devfs_drive);

	dev_super = load_super("devfs", 0);
	proc_super = load_super("devfs", 0);
	vfs_mount("/dev", dev_super);
	vfs_mount("/proc", proc_super);*/
}
