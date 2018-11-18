#include <vfs.h>
#include <print.h>
#include <kmalloc.h>
#include <umemlay.h>
#include <uload.h>
#include <umove.h>
#include <map.h>
#include <psm.h>

void __attribute__((noreturn)) transfer_to_user(void)
{
	static IF_REGS uregs = {
		.pc = USER_TEXT_BEG,
		.sp = USER_STACK_END,
		.cs = 0x001b,
		.ss = 0x0023,
		.ds = 0x0023,
		.es = 0x0023,
	};
	move_to_user(&uregs);
}

int load_user_program(const char *name)
{
	FILE *f = kmalloc_for(FILE);
	int res = open(f, name, OPEN_RD);
	if (res)
		goto out_free;

	void *addr = (void *) USER_TEXT_BEG;
	res = mmap(f, addr, f->f_size, MMAP_US);
	if (res < 0)
		goto out_close;
	else
		res = 0;

	unsigned long stkbtm = USER_STACK_END;
	for (int i = 0; i < 0xd00; i++)
		map(stkbtm -= PGSIZE, alloc_ppage() | PG_P | PG_W | PG_U);

out_close:
	close(f);
out_free:
	kfree(f);
	return res;
}
