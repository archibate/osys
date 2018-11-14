#include <fs.h>
#include <string.h>
#include <panic.h>
#include <kmalloc.h>
#include <print.h>


DIRENT *dir_find_file(DIR *dir, const char *name)
{
	list_foreach(le, dir->d_ents) {
		DIRENT *de = list_entry(DIRENT, e_list, le);
		//printf("dir_find_file: matching %s with %s\n", de->e_name, name);
		if (!strcmp(de->e_name, name)) {
			//printf("dir_find_file: found\n");
			return de;
		}
	}
	//printf("dir_find_file: not found\n");
	return 0;
}


FILE *open_inode(INODE *inode)
{
	return inode->i_op->open(inode);
}

FILE *open_in_dir(DIR *dir, const char *name)
{
	DIRENT *de = dir_find_file(dir, name);
	if (!de)
		return 0;

	FILE *file = open_inode(de->e_inode);

	return file;
}

void close(FILE *file)
{
	file->f_ops->close(file);
}

int read(FILE *file, char *buf, size_t size)
{
	return file->f_ops->read(file, buf, size);
}


SUPER *load_super(const char *fsname, void *arg)
{
	FSDRIVE *drive = get_fsdrive(fsname);
	if (!drive)
		return 0;
	return drive->load_super(arg);
}

void free_super(SUPER *sb)
{
	sb->s_ops->free_super(sb);
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
