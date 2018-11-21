#include <vfs.h>
#include <print.h>
#include <panic.h>
#include <kmalloc.h>
#include <umemlay.h>
#include <uload.h>
#include <umove.h>
#include <sched.h>
#include <schpcb.h>
#include <map.h>
#include <psm.h>
#include <mmu.h>
#include <memory.h>
#include <exf.h>

void __attribute__((noreturn)) transfer_to_user(void)
{
	static IF_REGS uregs = {
		.pc = USER_TEXT_BEG,
		.sp = USER_STACK_END,
		.eflags = 0x202,
		.cs = 0x001b,
		.ss = 0x0023,
		.ds = 0x0023,
		.es = 0x0023,
	};
	move_to_user(&uregs);
}

#define uload_map_psm_page(addr) map((addr), alloc_ppage() | PG_PSM | PG_P | PG_W | PG_U)

int load_user_program_fc(FILE *f)
{
	void *addr = (void *) USER_TEXT_BEG;

	assert((ssize_t) f->f_size > 0);

	EXF_HEADER exfhdr;

	int res = read(f, (void*)&exfhdr, sizeof(exfhdr));
	assert(exfhdr.x_magic == EXF_MAGIC);
	assert(res == sizeof(exfhdr));

	for (size_t off = 0; off < exfhdr.x_size; off += PGSIZE)
		uload_map_psm_page((unsigned long)addr + off);
	res = seek(f, exfhdr.x_off, SEEK_SET);
	assert(!res);
	res = read(f, addr, exfhdr.x_size);
	if (res > 0)
		res = 0;

	close(f);
	kfree(f);

	unsigned long stkbtm = USER_STACK_END;
	for (int i = 0; i < exfhdr.x_stkpgs; i++) // todo: move this to page_fault
		uload_map_psm_page(stkbtm -= PGSIZE);

	return res;
}

int load_user_program(const char *name)
{
	FILE *f = kmalloc_for(FILE);
	int res = open(f, name, OPEN_RD);
	if (res)
		goto out_free;

	res = load_user_program_fc(f);
out_free:
	kfree(f);
	return res;
}

void user_proc_destroy(void)
{
	unmap_free_psm_non_global_pages();
}
