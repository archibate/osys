#include <fs.h>
#include <uload.h>
#include <vfs.h>
#include <uload.h>
#include <atoi.h>
#include <memory.h>
#include <print.h>
#include <string.h>
#include <vacall.h>
#include <kmalloc.h>
#include <errno.h>
#include <panic.h>

int gview_main(const char *name);

int shell_ls(const char *name)
{
	if (!name)
		name = ".";
	printf("\nls of %s:\n\n", name);

	DIR *dir = kmalloc_for(DIR);
	int res = opendir(dir, name, OPEN_RD);
	if (res) {
		printf("opendir(%s): %m\n", name, res);
		goto out_free;
	}

	list_foreach(le, dir->d_ents)
	{
		DIRENT *de = list_entry(DIRENT, e_list, le);
		INODE *inode = de->e_inode;
		unsigned int type = inode->i_type;
		unsigned int attr = inode->i_attr;
		unsigned int eattr = inode->i_fat.ie_attr;
		unsigned int size = inode->i_size;
		printf(" %c%c%c%c"
				, "-dcb"[type]
				, attr  & INODE_RD    ? 'r' : '-'
				, attr  & INODE_WR    ? 'w' : '-'
				, attr  & INODE_EX    ? 'x' : '-'
				);
		printf("  %c%c%c%c%c%c"
				, eattr & FAT_RDONLY  ? 'R' : '-'
				, eattr & FAT_HIDDEN  ? 'H' : '-'
				, eattr & FAT_SYSTEM  ? 'S' : '-'
				, eattr & FAT_VOLLAB  ? 'V' : '-'
				, eattr & FAT_ARCHIV  ? 'A' : '-'
				, eattr & FAT_SUBDIR  ? 'D' : '-'
			        );

		int k = 0;
		while (size > 4608)
			size = (size + 512) / 1024, k++;

		if (type == INODE_DIR)
			printf("   <DIR>");
		else
			printf("   %-4d%c", size, "BKMG"[k]);

		printf("    %s\n", de->e_name);
	}
	printf("\n");

	closedir(dir);
out_free:
	kfree(dir);

	return 0;
}

int shell_exec(const char *name)
{
	int res = load_user_program(name);
	if (res) {
		printf("load_user_program(%s): %m\n", name, res);
		goto out;
	}

	transfer_to_user();

out:
	return res;
}

int shell_type(const char *name)
{
	FILE *f = kmalloc_for(FILE);
	int res = open(f, name, OPEN_RD);
	if (res) {
		printf("open(%s): %m\n", name, res);
		return res;
	}

	int size = f->f_size;
	char *buf = kmalloc(size);
	int len = read(f, buf, size);
	if (len < 0) {
		printf("read(%s): %m\n", name, res);
		return len;
	}

	printf("%.*s\n", len, buf);
	close(f);
	kfree(f);

	return 0;
}

int shell_cat(const char *name)
{
	FILE *f = kmalloc_for(FILE);
	int res = open(f, name, OPEN_RD);
	if (res) goto out_free;

	int ch;
	while ((ch = getch(f)) != EOF) {
		printf("%c", ch);
	}

	close(f);
out_free:
	kfree(f);

	return 0;
}

int shell_catkbd(void)
{
	FILE *f = kmalloc_for(FILE);
	int res = open(f, "/dev/kbd0", OPEN_RD);
	if (res) goto out_free;

	while (1) {
		unsigned int ch = getch(f);
		prrdtsc("G");
		putchar(ch);
	}

	close(f);
out_free:
	kfree(f);

	return 0;
}

int shell_map(	const char *name, const char *addrstr,
		const char *sizestr, const char *options)
{
	unsigned int mattr = 0, oattr = OPEN_RD;

	if (options && strfind(options, 'w') != -1) {
		mattr |= MMAP_WR;
		oattr |= OPEN_WR;
	}

	void *addr = (void *) strtol(addrstr, 0, 16);
	long size = strtol(sizestr, 0, 16);

	FILE *f = kmalloc_for(FILE);
	int res = open(f, name, oattr);
	if (res) {
		printf("open(%s): %m\n", name, res);
		goto out_free;
	}

	size = mmap(f, addr, size, mattr);
	if (size < 0) {
		printf("mmap(%s): %m\n", name, (int) size);
		goto out_close;
	}

	printf("mapped %s as r%c at %#p:%#x\n", name
			, mattr & MMAP_WR ? 'w' : 'o'
			, addr, size);

out_close:
	close(f);
out_free:
	kfree(f);
	return res;
}


#define ARGC_MAX 10
#define CSPACE " \t\n"

char **parse_argv(char *cmd, int *pargc)
{
	int i, argc = 0;
	char **argv = kmalloc((ARGC_MAX+1) * sizeof(const char *));
	cmd = strtrim(cmd, CSPACE);
	while ((i = strfindin(cmd, CSPACE)) != -1) {
		assert(argc < ARGC_MAX);
		cmd[i] = 0;
		argv[argc++] = cmd;
		cmd = strskip(cmd + i + 1, CSPACE);
	}
	argv[argc++] = cmd;
	argv[*pargc = argc] = 0;
	return argv;
}

int exec_cmd(int argc, char *const *argv)
{
#define SHCMD(name, argc_min, argc_max) {#name, shell_##name, argc_min, argc_max}
	struct command {
		char *name;
		int (*fun)();
		int argc_min, argc_max;
	} shcmds[] = {
		SHCMD(ls, 0,1),
		SHCMD(type, 1,1),
		SHCMD(cat, 1,1),
		SHCMD(catkbd, 0,0),
		SHCMD(map, 3,4),
		SHCMD(exec, 1,1),
		{"cd", setcwd, 1,1},
		{"gview", gview_main, 1,1},
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

	printf("sh: command not found: %s\n", name);
	return -E_NO_SRCH;
}

int shell_main(void)
{
	FILE *f = kmalloc_for(FILE);
	int res = open(f, "/dev/kbd0", OPEN_RD);
	if (res) {
		printf("open(/dev/kbd0): %m\n", res);
		return res;
	}

	while (1) {
		printf("> ");
		char *cmdl = getline(f);
		int argc;
		char **argv = parse_argv(cmdl, &argc);
		kfree(cmdl);
		if (argv[0])
			res = exec_cmd(argc, argv);
		kfree(argv);
	}

	close(f);
	kfree(f);

	return res;
}


void init_shell(void)
{
	setcwd("/root");
	shell_type("/dev/welcome");

	// change bootsect.asm line 9 to 0x103 to have a complete view of 2s2s.jpg
	//gview_main("2s2s.jpg");

	shell_exec("/bin/splash.com");
	shell_main();
}
