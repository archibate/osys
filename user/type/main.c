#include <stdio.h>
#include <unistd.h>
#include <malloc.h>

int main(const char *path)
{
	FILE *f = path[0] ? fopen(path, "r") : stdin;
	if (!f) {
		fprintf(stderr, "type: cannot open %s\n", path);
		exit(-1);
	}
	int size = tellsize(f->f_fd);
	char *buf = malloc(size);
	fread(buf, size, 1, f);
	fwrite(buf, size, 1, stdout);
	if (path[0])
		fclose(f);
	return 0;
}
