#include <fs.h>
#include <string.h>
#include <errno.h>
#include <memory.h>
#include <panic.h>
#include <kmalloc.h>
#include <print.h>


/*INODE *create_inode(SUPER *sb)
{
	INODE *inode = alloc_inode(sb);
	int res = inode->i_ops->create(inode);
	if (res) {
		free_inode(sb);
		return 0;
	}
	return inode;
}

void destroy_inode(INODE *inode)
{
	int res = inode->i_ops->remove(inode);
	assert(!res);
}*/



/*DIRENT *dir_find_entry(LIST_HEAD dents, const char *name)
{
	list_foreach(le, dents) {
		DIRENT *de = list_entry(DIRENT, e_list, le);
		if (!strcmp(de->e_name, name))
			return de;
	}
	return 0;
}*/

int simple_close(__attribute__((unused)) FILE *f)
{
	return 0;
}

int simple_closedir(__attribute__((unused)) DIR *d)
{
	return 0;
}

DIRENT *simple_dirfind(DIR *dir, const char *name)
{
	list_foreach(le, dir->d_ents) {
		DIRENT *de = list_entry(DIRENT, e_list, le);
		if (!strcmp(de->e_name, name))
			return de;
	}
	return 0;
}

DIRENT *dirfind(DIR *dir, const char *name)
{
	return dir->d_ops->dirfind(dir, name);
}

DIRENT *dir_locate_entry(DIR *_dir, const char *_name)
{
	int len = strlen(_name) + 1;
	char *name = kmalloc(len);
	memcpy(name, _name, len);

	DIR *dir = _dir;

	int i;
	while ((i = strfind(name, '/')) != -1) {
		name[i] = 0;
		DIRENT *de = dirfind(dir, name);
		if (!de)
			return 0;
		if (dir != _dir) kfree(dir);
		dir = kmalloc_for(DIR);
		inode_opendir(dir, de->e_inode, OPEN_RD);
		name += i + 1;
	}

	if (dir != _dir) kfree(dir);
	DIRENT *de = dirfind(dir, name);
	return de;
}

int open_in(FILE *file, DIR *indir, const char *name, unsigned int oattr)
{
	DIRENT *de = dir_locate_entry(indir, name);
	if (!de)
		return -ENOENT;

	return inode_open(file, de->e_inode, oattr);
}



int inode_open(FILE *file, INODE *inode, unsigned int oattr)
{
	if ((oattr & OPEN_DIR) && !(inode->i_attr & INODE_DIR)) {
		return -ENOTDIR;
	}
	if (!(oattr & OPEN_DIR) && (inode->i_attr & INODE_DIR)) {
		return -EISDIR;
	}
	if ((oattr & OPEN_WR) && !(inode->i_attr & INODE_WR)) {
		return -EACCES;
	}
	if ((oattr & OPEN_RD) && !(inode->i_attr & INODE_RD)) {
		return -EACCES;
	}

	return inode->i_fops->open(file, inode, oattr);
}

void close(FILE *file)
{
	file->f_ops->close(file);
}

long seek(FILE *file, long offset, int whence)
{
	return file->f_ops->seek(file, offset, whence);
}

/*int tell(FILE *file, int what)
{
	return file->f_ops->tell(file, what);
}*/

int read(FILE *file, char *buf, size_t size)
{
	return file->f_ops->read(file, buf, size);
}

int write(FILE *file, const char *buf, size_t size)
{
	return file->f_ops->write(file, buf, size);
}

unsigned int getch(FILE *file)
{
	return file->f_ops->getch(file);
}

int fsync(FILE *file)
{
	return file->f_ops->fsync(file);
}

int putch(FILE *file, unsigned char ch)
{
	return file->f_ops->putch(file, ch);
}

size_t glinesize(FILE *file)
{
	return file->f_ops->glinesize(file);
}

char *getline(FILE *file)
{
	return file->f_ops->getline(file);
}

int mmap(FILE *f, void *p, size_t size, unsigned int mattr)
{
	if ((((unsigned long)p) & PGATTR) != 0)
		return -EINVAL;

	return f->f_ops->mmap(f, p, size, mattr);
}




INODE *alloc_inode(SUPER *sb)
{
	return sb->s_ops->alloc_inode(sb);
}

void free_inode(INODE *inode)
{
	inode->i_sb->s_ops->free_inode(inode);
}

void free_super(SUPER *sb)
{
	sb->s_ops->free_super(sb);
}

SUPER *load_super(const char *fsname, void *arg)
{
	FSDRIVE *drive = get_fsdrive(fsname);
	if (!drive)
		return 0;
	return drive->load_super(arg);
}



FSDRIVE *fsdrives = 0;

FSDRIVE *get_fsdrive(const char *name)
{
	FSDRIVE *curr = fsdrives;
	while (curr) {
		if (!strcmp(curr->name, name))
			return curr;
		curr = curr->next;
	}
	return 0;
}

void add_fsdrive(FSDRIVE *fs)
{
	assert(!get_fsdrive(fs->name));
	fs->next = fsdrives;
	fsdrives = fs;
}
