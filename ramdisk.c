#include <vfs.h>
#include <print.h>
#include <panic.h>


SUPER *rd_sb;

void init_ramdisk(void)
{
	extern char ramdisk[];
	rd_sb = load_super("ramfs", ramdisk);

	printf("ls of /:\n");
	list_foreach(le, rd_sb->s_root->d_ents) {
		DIRENT *de = list_entry(DIRENT, e_list, le);
		printf("\t%s\n", de->e_name);
	}

	FILE *f = open_in_dir(rd_sb->s_root, "copying.txt");
	assert(f);
	char buf[37];
	read(f, buf, sizeof(buf));
	printf("file content of copying.txt:\n");
	printf("%.*s\n", sizeof(buf), buf);
	close(f);

	free_super(rd_sb);
}
