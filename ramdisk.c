#include <fs.h>
#include <vfs.h>
#include <print.h>
#include <kmalloc.h>
#include <panic.h>

int shell_ls(const char *name)
{
	printf("\nls of %s:\n\n", name);
	DIR *dir = kmalloc_for(DIR);
	int res = opendir(dir, name, OPEN_RD);
	if (res) {
		printf("opendir(%s): %m\n", name, res);
		return res;
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
	kfree(dir);

	return 0;
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


void init_shell(void)
{
	setcwd("/");

	shell_type("/dev/welcome");

	shell_ls("/etc");
	shell_ls("/dev");
}
