#include <stdio.h>
#include <unistd.h>
#include <malloc.h>

int main(const char *path)
{
	FILE *f = path[0] ? fopen(path, "r") : stdin;
	if (!f) {
		fprintf(stderr, "cat: cannot open %s\n", path);
		exit(-1);
	}
	char buf[BUFSIZ];
	while (fgets(buf, sizeof(buf), f))
		puts(buf);
	if (path[0])
		fclose(f);
	return 0;
}
