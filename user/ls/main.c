#include <stdio.h>
#include <dirent.h>
#include <unistd.h>

int main(const char *path)
{
	int i = opendir(path, OPEN_RD);
	if (i < 0) {
		fprintf(stderr, "ls: %s: %m\n", path, i);
		exit(-1);
	}
	DIRENT ent;
	while (!readdir(i, &ent)) {
		printf("%s\n", ent.e_name);
	}
	closedir(i);
	return 0;
}
