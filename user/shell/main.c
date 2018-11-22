#include <stdio.h>
#include <string.h>
#include <memory.h>
#include <malloc.h>
#include <vacall.h>
#include <errno.h>
#include <assert.h>
#include <unistd.h>

#define LINE_MAX 1024
#define CSPACE " \t\n"

char *get_binary_name(const char *name)
{
	unsigned long len = strlen(name);
	char *path = malloc(5 + len + 4 + 1);
	memcpy(path, "/bin/", 5);
	memcpy(path + 5, name, len);
	memcpy(path + 5 + len, ".exf", 4);
	path[5 + len + 4] = 0;
	return path;
}

int echo(const char *s)
{
	printf("%s\n", s);
	return 0;
}

int run_cmd(char *cmdl)
{
	struct command {
		char *name;
		int (*fun)(const char *arg);
	} shcmds[] = {
		{"cd", chdir},
		//{"echo", echo},
	};

	int i = strfindlin(cmdl, CSPACE);
	const char *arg = strskipin(cmdl + i, CSPACE);

	cmdl[i] = 0;
	const char *name = cmdl;

	for (i = 0; i < sizeof(shcmds) / sizeof(shcmds[0]); i++) {
		if (!strcmp(shcmds[i].name, name)) {
			return shcmds[i].fun(arg);
		}
	}

	char *path = get_binary_name(name);

	int res = stexecv1(path, arg);

	if (res == -ENOENT)
		printf("sh: command not found: %s\n", name);
	else if (res < 0)
		printf("sh: %s: %m\n", path, res);
	else
		res = waitpid(res);

	return res;
}


int main(void)
{
	printf("\nOSYS shell v0.1\n");

	char cmdl[LINE_MAX + 1];

	while (1) {
		printf("\n> ");

		fgets(cmdl, LINE_MAX, stdin);
		run_cmd(strtrim(cmdl, CSPACE));
	}

	return 0;
}
