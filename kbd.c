#include <fs.h>
#include <devfs.h>
#include <memory.h>
#include <mkefifo.h>
#include <print.h>

extern
EFIFO keybd_efifo;


void init_kbd(void)
{
	make_efifo_dev("kbd0", &keybd_efifo, INODE_CHR | INODE_RD);
}
