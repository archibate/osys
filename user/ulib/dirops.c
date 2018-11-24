#include <dirent.h>
#include <malloc.h>

int opendir_s(DIR **pf, const char *name, const char *type)
{
	*pf = malloc_for(DIR);
	int res = fopen_i(*pf, name, type);
	if (res) {
		free(*pf);
		*pf = 0;
	}
	return res;
}
