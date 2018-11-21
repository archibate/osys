#include <stdio.h>
#include <string.h>
#include <memory.h>
#include <malloc.h>
#include <unistd.h>

static
void file_parse_oattr(FILE *f, const char *type)
{
	unsigned int oattr = 0;

	if (strfind(type, 'r') != -1)
		oattr |= OPEN_RD;
	if (strfind(type, 'w') != -1)
		oattr |= OPEN_WR;

	f->f_oattr = oattr;
}

int fopen_i(FILE *f, const char *name, const char *type)
{
	bzero(f, sizeof(FILE));

	file_parse_oattr(f, type);

	int res = f->f_fd = open(name, f->f_oattr);
	if (res > 0)
		res = 0;

	return res;
}

FILE *fopen(const char *name, const char *type)
{
	FILE *fp = malloc_for(FILE);
	int res = fopen_i(fp, name, type);
	if (res) {
		free(fp);
		fp = NULL;
	}
	return fp;
}

int fopen_s(FILE **pf, const char *name, const char *type)
{
	*pf = malloc_for(FILE);
	int res = fopen_i(*pf, name, type);
	if (res) {
		free(*pf);
		*pf = 0;
	}
	return res;
}

int fclose_i(FILE *f)
{
	return close(f->f_fd);
}

int fclose(FILE *f)
{
	int res = fclose_i(f);
	free(f);
	return res;
}
