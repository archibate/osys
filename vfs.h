#pragma once


#include <fs.h>


extern
DIR *root, *cdir;

static inline
void vfs_setroot(DIR *new_root)
{
	root = new_root;
}

void vfs_mount(const char *name, SUPER *sb);
DIRENT *locate_entry(const char *name);
int open(FILE *file, const char *name, unsigned int oattr);
int opendir(DIR *dir, const char *name, unsigned int oattr);
int setcwd(const char *new_cwd);
