#include <stdio.h>
#include <string.h>
#include <memory.h>
#include <malloc.h>
#include <unistd.h>

static
unsigned int file_parse_oattr(FILE *f, const char *type)
{
	unsigned int oattr = 0;

	if (strfind(type, 'r') != -1)
		oattr |= OPEN_RD;
	if (strfind(type, 'w') != -1)
		oattr |= OPEN_WR;

	/*if (strfind(type, 'b') == -1) {
		f->f_linebuf = malloc_for(FIFO);
		fifo_init(f->f_linebuf);
	}*/

	f->f_bpos = 0;
	f->f_bsize = 0;

	return oattr;
}

int fopen_i(FILE *f, const char *name, const char *type)
{
	unsigned int oattr = file_parse_oattr(f, type);

	bzero(f, sizeof(FILE));

	int res = f->f_fd = open(name, oattr);
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
	/*if (f->f_linebuf)
		free(f->f_linebuf);*/
	return close(f->f_fd);
}

int fclose(FILE *f)
{
	int res = fclose_i(f);
	free(f);
	return res;
}
