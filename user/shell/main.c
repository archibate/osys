#include <stdio.h>
#include <string.h>
#include <memory.h>
#include <malloc.h>
#include <vacall.h>
#include <errno.h>
#include <assert.h>
#include <unistd.h>

#define LINE_MAX 1024
#define ARGC_MAX 10
#define CSPACE " \t\n"

void parse_argv(char *cmd, int *pargc, char **argv)
{
	int i, argc = 0;
	cmd = strtrim(cmd, CSPACE);
	while ((i = strfindin(cmd, CSPACE)) != -1) {
		assert(argc < ARGC_MAX);
		cmd[i] = 0;
		argv[argc++] = cmd;
		cmd = strskip(cmd + i + 1, CSPACE);
	}
	argv[argc++] = cmd;
	argv[*pargc = argc] = 0;
}

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

int run_cmd(int argc, char *const *argv)
{
	struct command {
		char *name;
		int (*fun)();
		int argc_min, argc_max;
	} shcmds[] = {
		{"cd", chdir, 1,1},
		{"echo", puts, 1,1},
	};

	char *name = argv[0]; argc--, argv++;
	for (int i = 0; i < sizeof(shcmds) / sizeof(shcmds[0]); i++) {
		if (!strcmp(shcmds[i].name, name)) {
			if (argc > shcmds[i].argc_max || argc < shcmds[i].argc_min) {
				printf("sh: wrong arguments count\n");
				return -E_INVL_ARG;
			}
			return vacall(shcmds[i].fun, argc+1, (int*)argv);
		}
	}

	char *path = get_binary_name(name);

	int res = stexec(path);

	if (res == -E_NO_SRCH)
		printf("sh: command not found: %s\n", name);
	else if (res)
		printf("sh: stexec(%s): %m", path, res);

	return res;
}


int main(void)
{
	printf("\nOSYS shell v0.1\n");

	char s[LINE_MAX + 1];
	char *(argv[ARGC_MAX + 1]);
	int argc;

	while (1) {
		printf("\n> ");

		fgets(s, LINE_MAX, stdin);

		parse_argv(s, &argc, argv);
		if (argv[0]) run_cmd(argc, argv);
	}

	return 0;
}
