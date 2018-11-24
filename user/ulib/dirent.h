#pragma once

#include <unistd.h>

typedef struct U_DIRENT DIRENT;

static inline
int opendir(const char *name, unsigned int oattr)
{
	return open(name, oattr | OPEN_DIR);
}

int closedir(int fd)
{
	return close(fd);
}
