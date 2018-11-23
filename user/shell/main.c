#include <stdio.h>
#include <string.h>
#include <memory.h>
#include <malloc.h>
#include <vacall.h>
#include <errno.h>
#include <assert.h>
#include <unistd.h>
#include <atoi.h>

#define LINE_MAX 1024
#define CSPACE " \t\n"

int echo(const char *s)
{
	printf("%s\n", s);
	return 0;
}

int __attribute__((noreturn)) shexit(const char *val)
{
	exit(atoi(val));
}

void __attribute__((noreturn)) shexecap(const char *name, const char *arg)
{
	static char path[LINE_MAX];
	unsigned long len = strlen(name);
	memcpy(path, "/bin/", 5);
	memcpy(path + 5, name, len);
	memcpy(path + 5 + len, ".exf", 4);
	path[5 + len + 4] = 0;

	int res = execap(path, arg);

	if (res == -ENOENT)
		printf("sh: command not found: %d\n", name, res);
	else
		printf("sh: execap(%s): %m\n", path, res);
	exit(res);
}

const char *get_name_arg(char *cmdl) //{{{
{
	int i = strfindlin(cmdl, CSPACE);
	const char *arg = strskipin(cmdl + i, CSPACE);
	cmdl[i] = 0;
	return arg;
} //}}}

int start(char *cmdl) //{{{
{
	const char *arg = get_name_arg(cmdl);
	const char *name = cmdl;

	int pid = fork();

	if (!pid)
		shexecap(name, arg);

	static int started_nr = 0;
	printf("[%d] %d\n", ++started_nr, pid);

	return 0;
} //}}}

int run_cmd(char *cmdl)
{
	static struct command {
		char *name;
		int (*fun)(const char *arg);
	} shcmds[] = {
		{"cd", chdir},
		{"echo", echo},
		{"start", (void*)start},
		{"exit", shexit},
	};

	const char *arg = get_name_arg(cmdl);
	const char *name = cmdl;

	for (int i = 0; i < sizeof(shcmds) / sizeof(shcmds[0]); i++) {
		if (!strcmp(shcmds[i].name, name)) {
			return shcmds[i].fun(arg);
		}
	}

	int pid = fork();

	if (!pid)
		shexecap(name, arg);

	return waiton(pid, EVE_EXIT);
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
