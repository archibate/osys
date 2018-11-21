#include <mkpipe.h>

static
PIPE pip0;

void init_pip0(void)
{
	pipe_init(&pip0);
	make_pipe_dev("pip0", &pip0, INODE_CHR | INODE_RD | INODE_WR);
}
