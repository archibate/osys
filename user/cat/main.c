#include <stdio.h>
#include <unistd.h>

int main(const char *path)
{
	FILE *f = path[0] ? fopen(path, "r") : stdin;
	if (!f) {
		fprintf(stderr, "cat: cannot open file %s\n", path);
		exit(-1);
	}

	do puts(fgetrdbuf(f));
	while (!feof(f));
	fflush(stdout);

	if (path[0])
		fclose(f);
	return 0;
}
